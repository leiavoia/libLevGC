#include "ResourceMgr.h"

#include "../utilities/LogManager.h"
#include "cast.h"



namespace LGC {





ResourceMgr::ResourceMgr() : use_audio(true) {
	//textures = std::map< std::string, MapEntry<Texture> >();
	//class_registry = std::map< std::string, ResourceMgr::RegistrationForm >();
	Init();
	}


ResourceMgr* ResourceMgr::Inst() {
	static ResourceMgr* x = new ResourceMgr();
	return x;
	}



void ResourceMgr::Register( std::string class_name, register_func Load, register_func Dump ) {
	LogMgr::Write( LOGMSG_DEBUG, "Registering class '%s' as a resource client.", class_name.c_str() );
	class_registry[ class_name ] = RegistrationForm( Load, Dump );
	}





bool ResourceMgr::ReserveResourcesFor( std::vector< std::string > classnames ) {

	bool some_failed = false;

	for (unsigned int i = 0; i < classnames.size(); i++) {
		if ( ReserveResourcesFor( classnames[i] ) ) {
			some_failed = true;
			}
		}

	return some_failed;
	}




bool ResourceMgr::ReserveResourcesFor( std::string classname ) {
	std::map< std::string, RegistrationForm >::iterator x;
	x = class_registry.find( classname );

	// found! load resources
	if ( x != class_registry.end() ) {
		if ( x->second.holds == 0 ) { x->second.Load(); }
		x->second.holds++;
		return true;
		}

	// error - not found
	LogMgr::Write(
		LOGMSG_ERROR, "ResourceMgr::ReserveResourcesFor(\"%s\"): \"%s\" is not registered as a resource client. Resources will not be reserved",
		classname.c_str(),
		classname.c_str()
		);

	return false;
	}




bool ResourceMgr::DumpResourcesFor( std::vector< std::string > classnames ) {

	bool some_failed = false;

	for (unsigned int i = 0; i < classnames.size(); i++) {
		if ( DumpResourcesFor( classnames[i] ) ) {
			some_failed = true;
			}
		}

	return some_failed;
	}




bool ResourceMgr::DumpResourcesFor( std::string classname ) {
	std::map< std::string, RegistrationForm >::iterator x;
	x = class_registry.find( classname );

	// found! load resources
	if ( x != class_registry.end() ) {
		x->second.holds--;
		if ( x->second.holds == 0 ) {  x->second.Dump();  }
// 		else {
// 			LogMgr::Write( LOGMSG_DEBUG, "ResourceMgr::DumpResourcesFor(\"%s\"): Resources are still requested by another caller and will not be freed at this time.",classname.c_str() );
// 			}
		return true;
		}

	// error - not found
	LogMgr::Write(
		LOGMSG_ERROR, "ResourceMgr::DumpResourcesFor(\"%s\"): \"%s\" is not registered as a resource client. Resources will not be dumped",
		classname.c_str(),
		classname.c_str()
		);

	return false;
	}




bool ResourceMgr::Init() {
// 	static bool already_done = false;
//
// 	if (already_done) { return false; }
//
// 	if ( !LoadResourceXMLFile( "resource/resources.xml" ) ) {
// 		LogMgr::Write( LOGMSG_WARNING, "Main resource config file could not be properly loaded. You may experience graphic or audio problems." );
// 		return false;
// 		}
//
// 	// success!
// 	already_done = true;
	return true;
	}


bool ResourceMgr::LoadResourceXMLFile( const std::string& file ) {

	LogMgr::Write( LOGMSG_DEBUG, "Parsing Resoure Config File '%s'...", file.c_str() );

	// load the main resource conf file:
	TiXmlDocument doc( file.c_str() );
	if ( !doc.LoadFile() ) {
		LogMgr::Write( LOGMSG_ERROR, "Could not open config file '%s'. Error='%s'.", file.c_str(), doc.ErrorDesc() );
		return false;
		}

	// check for any other loading errors
	if ( doc.Error() ) {
		LogMgr::Write( LOGMSG_ERROR, "Problem loading '%s'. Error='%s'.", file.c_str(), doc.ErrorDesc() );
		return false;
		}

	// get the path to actual resource files based on the path to the XML file:
	file_path = file;
	int last_slash = file_path.find_last_of("/");
	file_path.erase( last_slash + 1 );

	ScanConfigForTextures( doc );
	ScanConfigForFonts( doc );
	ScanConfigForSounds( doc );
	ScanConfigForMusic( doc );
	ScanConfigForMaterials( doc );
	ScanConfigForLights( doc );

	LogMgr::Write( LOGMSG_DEBUG, "Resource config file '%s' parsing complete.", file.c_str() );
	return true;
	}





/*///////////////////////////////////////////////////////////////////////////

===========\/===========  GET / DUMP  FUNCTIONS  ==============\/============

///////////////////////////////////////////////////////////////////////////*/





Texture ResourceMgr::GetTexture( std::string handle, std::string id ) {

	if ( id.empty() ) { return Texture(); }

	// locate the texture in the map
	subtexture_info_iter subtexture = subtexture_info.find( handle );
	if ( subtexture == subtexture_info.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "Class '%s' requested an unknown texture '%s'. Returning NULL. (Resources Init()'ed?)", id.c_str(), handle.c_str() );
		return Texture();
		}

	// find parent
	textures_iter parent = textures.find( subtexture->second.parent );
	if ( parent == textures.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "Class '%s' referenced an unknown parent texture '%s'. Returning NULL. (Resources configured correctly?)", id.c_str(), subtexture->second.parent.c_str() );
		return Texture();
		}

	// Load if needed
	if ( parent->second.NotLoaded() ) {
		parent->second.resource = LoadTexture( file_path + parent->second.data, handle ); // "data" = file
		}

	// log the borrower
	parent->second.Hold(id);

	// TODO: switch to dynamic memory system?

	// create and return a subtexture stub
	AreaRect<int> bla = parent->second.resource.GetPixels();
	return Texture(
		handle,
		parent->second.resource.ID(),
		bla,
		(subtexture->second.area.Valid()) ? subtexture->second.area : bla // careful of tilable textures
		);
	}





void ResourceMgr::DumpTexture( const Texture& img,  std::string id ) {

	if ( id.empty() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpTexture() --> NULL typeid. Texture not dumped." );
		}

	// locate the subtexture in the map
	subtexture_info_iter subtexture = subtexture_info.find( img.Handle() );
	if ( subtexture == subtexture_info.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpTexture() --> Class '%s' requested to dump an unknown texture '%s'. No Action Taken. (Typo?)", id.c_str(), img.Handle().c_str() );
		return;
		}

	// find parent
	textures_iter parent = textures.find( subtexture->second.parent );
	if ( parent == textures.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpTexture() --> Subtexture '%s' referenced an unknown parent texture '%s'. No Action Taken. (Resources configured correctly?)", subtexture->first.c_str(), img.Handle().c_str() );
		return;
		}

	// release hold and dump if needed
	if ( parent->second.Release(id) ) {
//		LogMgr::Write( LOGMSG_DEBUG, "Texture dumped: '%s'.", handle.c_str() );
		const GLuint gl_id = parent->second.resource.ID();
		glDeleteTextures( 1, &gl_id );
		parent->second.resource = Texture();
		}
	}




Font ResourceMgr::GetFont( std::string handle, std::string id ) {

	if ( id.empty() ) { return Font(); }

	// locate the font in the map
	font_aliases_iter alias = font_aliases.find( handle );
	if ( alias == font_aliases.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "Class '%s' requested an unknown font alias '%s'. Returning NULL. (Resources Init()'ed?)", id.c_str(), handle.c_str() );
		return Font();
		}

	// find true font
	fonts_iter font = fonts.find( alias->second );
	if ( font == fonts.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::GetFont() --> Font alias '%s' referenced an unknown font '%s'. No Action Taken. (Resources configured correctly?)", alias->first.c_str(), alias->second );
		return Font();
		}

//	LogMgr::Write( LOGMSG_DEBUG, "Loading font '%s'... ", handle.c_str() );

	// Load if needed
	if ( font->second.NotLoaded() ) {
		FontFactory factory = FontFactory();
		// WARNING: don't forget to delete this!
		font->second.resource = factory.Make( (file_path + font->second.data.file).c_str() );
		if ( font->second.resource->Error() ) {
			LogMgr::Write( LOGMSG_ERROR, "Font file '%s' could not be located", (file_path + font->second.data.file).c_str() );
			delete font->second.resource;
			font->second.resource = NULL;
			return Font();
			}
		font->second.resource->SetFaceSize( font->second.data.size );
		if ( !(font->second.data.advance.empty()) ) {
			font->second.resource->Advance(  font->second.data.advance.c_str()  );
			}
		}

	// log the borrower
	font->second.Hold(id);

//	LogMgr::Write( LOGMSG_DEBUG, "Loaded font '%s' ", handle.c_str() );

	// create and return a font stub
	return Font( handle, font->second.resource );
	}





void ResourceMgr::DumpFont( Font& f, std::string id ) {

	if ( id.empty() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpFont() --> NULL typeid. Font not dumped." );
		}

	// locate the Font in the map
	font_aliases_iter alias = font_aliases.find( f.Handle() );
	if (f.Handle() == "NO_HANDLE") { return; }
	if ( alias == font_aliases.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpFont() --> Class '%s' requested to dump an unknown font alias '%s'. No Action Taken. (Internal Error: font's internal handle does not match Resource Manager's records [?] )", id.c_str(), f.Handle().c_str() );
		return;
		}

	// find true font
	fonts_iter font = fonts.find( alias->second );
	if ( font == fonts.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpFont() --> Font alias '%s' referenced an unknown font '%s'. No Action Taken. (Resources configured correctly?)", alias->first.c_str(), alias->second );
		return;
		}

	// release hold and dump if needed
	if ( font->second.Release(id) ) {
//		LogMgr::Write( LOGMSG_DEBUG, "Font dumped: '%s'.", f.Handle().c_str() );
		delete font->second.resource;
		font->second.resource = NULL;
		}

//	LogMgr::Write( LOGMSG_DEBUG, "Dumped font '%s' ", font->first.c_str() );
	}





Sound ResourceMgr::GetSound( std::string handle, std::string id ) {

	if ( id.empty() || !use_audio ) { return Sound(); }

	// locate the sound in the alias map
	sound_aliases_iter alias = sound_aliases.find( handle );
	if ( alias == sound_aliases.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "Class '%s' requested an unknown sound alias '%s'. Returning NULL. (Resources Init()'ed?)", id.c_str(), handle.c_str() );
		return Sound();
		}

	// find true sound
	sounds_iter sound = sounds.find( alias->second );
	if ( sound == sounds.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::GetSound() --> Sound alias '%s' referenced an unknown sound [#%d]. No Action Taken. (Resources configured correctly?)", alias->first.c_str(), alias->second );
		return Sound();
		}

//	LogMgr::Write( LOGMSG_DEBUG, "Loading sound '%s'... ", handle.c_str() );

	// Load if needed
	if ( sound->second.NotLoaded() ) {
		// WARNING: don't forget to delete this!
		sound->second.resource = Mix_LoadWAV(  (file_path + sound->second.data.file).c_str()  );
		if ( !(sound->second.resource) ) {
			LogMgr::Write( LOGMSG_ERROR, "Sound file '%s' could not be located or was an incompatible file type", (file_path + sound->second.data.file).c_str() );
			return Sound();
			}
		}

	// log the borrower
	sound->second.Hold(id);

//	LogMgr::Write( LOGMSG_DEBUG, "Loaded sound '%s' ", handle.c_str() );

	// create and return a sound stub
	return Sound( handle, sound->second.resource );
	}





void ResourceMgr::DumpSound( Sound& s, std::string id ) {

	if ( id.empty() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpSound() --> NULL id. Sound not dumped." );
		}

	// locate the Font in the map
	if (s.Handle() == "NO_HANDLE") { return; }
	sound_aliases_iter alias = sound_aliases.find( s.Handle() );
	if ( alias == sound_aliases.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpSound() --> Class '%s' requested to dump an unknown sound alias '%s'. No Action Taken. (Internal Error: sound's internal handle does not match Resource Manager's records [?] )", id.c_str(), s.Handle().c_str() );
		return;
		}

	// find true sound
	sounds_iter sound = sounds.find( alias->second );
	if ( sound == sounds.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpSound() --> Sound alias '%s' referenced an unknown sound '%s'. No Action Taken. (Resources configured correctly?)", alias->first.c_str(), alias->second );
		return;
		}

	// release hold and dump if needed
	if ( sound->second.Release(id) ) {
//		LogMgr::Write( LOGMSG_DEBUG, "Sound dumped: '%s'.", s.Handle().c_str() );
		Mix_FreeChunk( sound->second.resource );
		sound->second.resource = NULL;
		}

//	LogMgr::Write( LOGMSG_DEBUG, "Dumped sound [#%d] ", sound->first );
	}




MusicTrack ResourceMgr::GetTrack( std::string handle, std::string id ) {

	if ( id.empty() || !use_audio ) { return MusicTrack(); }

	// locate the sound in the alias map
	track_aliases_iter alias = track_aliases.find( handle );
	if ( alias == track_aliases.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "Class '%s' requested an unknown music track alias '%s'. Returning NULL. (Resources Init()'ed?)", id.c_str(), handle.c_str() );
		return MusicTrack();
		}

	// find true sound
	tracks_iter track = tracks.find( alias->second );
	if ( track == tracks.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::GetTrack() --> Track alias '%s' referenced an unknown music track [#%d]. No Action Taken. (Resources configured correctly?)", alias->first.c_str(), alias->second );
		return MusicTrack();
		}

//	LogMgr::Write( LOGMSG_DEBUG, "Loading music track '%s'... ", handle.c_str() );

	// Load if needed
	if ( track->second.NotLoaded() ) {
		// WARNING: don't forget to delete this!
		track->second.resource = Mix_LoadMUS(  (file_path + track->second.data.file).c_str()  );
		if ( !(track->second.resource) ) {
			LogMgr::Write( LOGMSG_ERROR, "Music track file '%s' could not be located or was an incompatible file type", (file_path + track->second.data.file).c_str() );
			return MusicTrack();
			}
		}

	// log the borrower
	track->second.Hold(id);

//	LogMgr::Write( LOGMSG_DEBUG, "Loaded music track '%s' ", handle.c_str() );

	// create and return a sound stub
	return MusicTrack( handle, track->second.resource );
	}





void ResourceMgr::DumpTrack( MusicTrack& t, std::string id ) {

	if ( id.empty() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpTrack() --> NULL id. Sound not dumped." );
		}

	// locate the Font in the map
	if (t.Handle() == "NO_HANDLE") { return; }
	track_aliases_iter alias = track_aliases.find( t.Handle() );
	if ( alias == track_aliases.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpTrack() --> Class '%s' requested to dump an unknown track alias '%s'. No Action Taken. (Internal Error: track's internal handle does not match Resource Manager's records [?] )", id.c_str(), t.Handle().c_str() );
		return;
		}

	// find true sound
	tracks_iter track = tracks.find( alias->second );
	if ( track == tracks.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpTrack() --> Track alias '%s' referenced an unknown track '%s'. No Action Taken. (Resources configured correctly?)", alias->first.c_str(), alias->second );
		return;
		}

	// release hold and dump if needed
	if ( track->second.Release(id) ) {
//		LogMgr::Write( LOGMSG_DEBUG, "Track dumped: '%s'.", t.Handle().c_str() );
		Mix_FreeMusic( track->second.resource );
		track->second.resource = NULL;
		}

//	LogMgr::Write( LOGMSG_DEBUG, "Dumped music track [#%d] ", track->first );
	}





Texture ResourceMgr::LoadTexture( std::string file, std::string handle ) {

	Texture t = Texture();

	if ( file.empty() ) { return t; }

	/* Create storage space for the texture */
	SDL_Surface* img = IMG_Load( file.c_str() );

	if ( img ) {

		// check for ^2 size. code returns 1 if not a ^2
		if (  (img->h & ( img->h-1 ))  ||  (img->w & ( img->w-1 )) ) {
			LogMgr::Write(LOGMSG_ERROR, "Loaded image is not a power of two: (%d width, %d height). Returning dummy stub", img->w, img->h );
			return t;
			}

//		THIS GARBAGE IS FOR COPYING PORTIONS OF SURFACES WITH ALPHA INTACT
// 		SDL_Surface* fart = SDL_CreateRGBSurface(SDL_HWSURFACE, 16, 16, 32, img->format->Rmask, img->format->Gmask, img->format->Bmask, img->format->Amask);;
// 		SDL_Surface* newimg = SDL_DisplayFormatAlpha( fart );
// 		SDL_Surface* tempimg = SDL_DisplayFormatAlpha( img );
// 		SDL_Rect srcrect = {16,16,16,16};
// 		SDL_BlitSurface(tempimg, &srcrect, newimg, NULL);
// 		SDL_FreeSurface(tempimg);

		// convert the surface to a common BGRA format
		SDL_Surface* newimg = SDL_DisplayFormatAlpha( img );

		// free original image
		SDL_FreeSurface( img );

		GLuint texture = 0;
		GLint previous = 0;
		glGetIntegerv( GL_TEXTURE_BINDING_2D, &previous );

		/* Create The Texture */
		glGenTextures( 1, &texture );

		//LogMgr::Write(LOGMSG_DEBUG, "Generated texture index: %d (ERRORS: %d) ", (int)texture, (int)glGetError() );

		/* Typical Texture Generation Using Data From The Bitmap */
		glBindTexture( GL_TEXTURE_2D, texture );

		/* Generate The Texture */
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			4,
			newimg->w,
			newimg->h,
			0,
			GL_BGRA, // NOTE <-------------------------
			GL_UNSIGNED_BYTE,
			newimg->pixels
			);

		/* Linear Filtering */
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); //GL_NEAREST
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		//GLint p[4] = {0,0,0,0};
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

		AreaRect<int> bla(0,0,newimg->w,newimg->h);
		t = Texture( handle, (int)texture, bla, bla	);

		// free new image
		SDL_FreeSurface( newimg );

		//LogMgr::Write(LOGMSG_DEBUG, "Texture loaded: %s", file.c_str() );

		// back to normal
		glBindTexture( GL_TEXTURE_2D, previous );

		return t;
		}

	LogMgr::Write(LOGMSG_ERROR, "Image '%s' is NULL in ResourceMgr::LoadTexture(). (File '%s' probably could not be located). Returning dummy stub.", handle.c_str(), file.c_str()  );
	return t;
	}




Material ResourceMgr::GetMaterial( std::string handle, std::string id ) {

	if ( id.empty() ) { return Material(); }

	// locate in the alias map
	material_aliases_iter alias = material_aliases.find( handle );
	if ( alias == material_aliases.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "Class '%s' requested an unknown material alias '%s'. Returning stub. (Resources Init()'ed?)", id.c_str(), handle.c_str() );
		return Material();
		}

	// find true sound
	materials_iter m = materials.find( alias->second );
	if ( m == materials.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::GetMaterial() --> Material alias '%s' referenced an unknown material [#%d]. No Action Taken. (Resources configured correctly?)", alias->first.c_str(), alias->second );
		return Material();
		}

//	LogMgr::Write( LOGMSG_DEBUG, "Loading music track '%s'... ", handle.c_str() );

	// Load if needed
	if ( m->second.NotLoaded() ) {
		// NOTE: we're just returning a pointer to an object we already stored at config_file load time.
		m->second.resource = &m->second.data; // he he
		}

	// log the borrower
	m->second.Hold(id);

//	LogMgr::Write( LOGMSG_DEBUG, "Loaded music track '%s' ", handle.c_str() );

	// create and return an object
	return Material::CloneAndName( *m->second.resource, handle );
	}





void ResourceMgr::DumpMaterial( Material& x, std::string id ) {

	if ( id.empty() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpMaterial() --> NULL id. Material not dumped." );
		}

	// locate the resource in the map
// 	if (x.Handle() == "NO_HANDLE") { return; }
	material_aliases_iter alias = material_aliases.find( x.Name() );
	if ( alias == material_aliases.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpMaterial() --> Class '%s' requested to dump an unknown material alias '%s'. No Action Taken. (Internal Error: materials's internal handle does not match Resource Manager's records [?] )", id.c_str(), x.Name().c_str() );
		return;
		}

	// find true material
	materials_iter m = materials.find( alias->second );
	if ( m == materials.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpMaterial() --> material alias '%s' referenced an unknown material '%s'. No Action Taken. (Resources configured correctly?)", alias->first.c_str(), alias->second );
		return;
		}

	// release hold and dump if needed
	if ( m->second.Release(id) ) {
		// NOTE: Delete not needed. Pointer is not from heap.
		m->second.resource = NULL;
		}

	}



Light ResourceMgr::GetLight( std::string handle, std::string id ) {

	if ( id.empty() ) { return Light(); }

	// locate in the alias map
	light_aliases_iter alias = light_aliases.find( handle );
	if ( alias == light_aliases.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "Class '%s' requested an unknown light alias '%s'. Returning stub. (Resources Init()'ed?)", id.c_str(), handle.c_str() );
		return Light();
		}

	// find true sound
	lights_iter m = lights.find( alias->second );
	if ( m == lights.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::GetLight() --> Light alias '%s' referenced an unknown light [#%d]. No Action Taken. (Resources configured correctly?)", alias->first.c_str(), alias->second );
		return Light();
		}

//	LogMgr::Write( LOGMSG_DEBUG, "Loading music track '%s'... ", handle.c_str() );

	// Load if needed
	if ( m->second.NotLoaded() ) {
		// NOTE: we're just returning a pointer to an object we already stored at config_file load time.
		m->second.resource = &m->second.data; // he he
		// create a light number
		m->second.resource->SnagANewLightNumber();
		}

	// log the borrower
	m->second.Hold(id);

//	LogMgr::Write( LOGMSG_DEBUG, "Loaded music track '%s' ", handle.c_str() );

	// FIXME
	// return an object
	return Light::CloneAndRename( *m->second.resource, handle );

	}



void ResourceMgr::DumpLight( Light& x, std::string id ) {

	if ( id.empty() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpLight() --> NULL id. Light not dumped." );
		}

	// locate the resource in the map
// 	if (x.Handle() == "NO_HANDLE") { return; }
	light_aliases_iter alias = light_aliases.find( x.Name() );
	if ( alias == light_aliases.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpLight() --> Class '%s' requested to dump an unknown light alias '%s'. No Action Taken. (Internal Error: light's internal handle does not match Resource Manager's records [?] )", id.c_str(), x.Name().c_str() );
		return;
		}

	// find true resource
	lights_iter m = lights.find( alias->second );
	if ( m == lights.end() ) {
		LogMgr::Write( LOGMSG_ERROR, "ResourceMgr::DumpLight() --> light alias '%s' referenced an unknown light '%s'. No Action Taken. (Resources configured correctly?)", alias->first.c_str(), alias->second );
		return;
		}

	// release hold and dump if needed
	if ( m->second.Release(id) ) {
		// free up the light number
		m->second.resource->RetireMyLightNumber();
		// NOTE: Delete not needed. Pointer is not from heap.
		m->second.resource = NULL;
		}
	}


/*///////////////////////////////////////////////////////////////////////////

=======\/==========  XML CONFIG SCANNING FUNCTIONS  =============\/==========

///////////////////////////////////////////////////////////////////////////*/





void ResourceMgr::ScanConfigForLights( TiXmlDocument& doc ) {

	static int name_counter = 0;

	// find LIGHTS tag
	TiXmlNode* xmllights = doc.FirstChild( "lights" );
	if ( !xmllights ) {
		LogMgr::Write( LOGMSG_ERROR, "No LIGHTS tag found in config file '%s'. Lights will not load!", doc.Value() );
		return;
		}

	// foreach light
	for ( TiXmlElement* el = xmllights->FirstChildElement("light"); el; el = el->NextSiblingElement("light") ) {

// 		<light>
// 			<alias>default</alias> <!-- NOTE: this is different than the other aliases -->
// 			<ambient R="0.4" G="0.4" B="0.4" A="1" />
// 			<diffuse R="1" G="1" B="1" A="1" />
// 			<specular R="1" G="1" B="1" A="1" />
// 			<position X="0" Y="0" Z="0" positional="0" /><!-- positional=0 is a far away light with constant attenuation-->
// 			<spot_direction X="0" Y="0" Z="0" />
// 			<spot_radius>180</spot_radius><!-- 0 - 90, 180=no_spot, 180 default -->
// 			<spot_exponant>2.0</spot_exponant>
// 			<constant_attenuation>1.5</constant_attenuation>
// 			<linear_attenuation>0.5</linear_attenuation>
// 			<quadratic_attenuation>0.2</quadratic_attenuation>
// 		</light>

		// create the stub
		Light light = Light();

		// ambient
		TiXmlElement* ambient = el->FirstChildElement( "ambient" );
		if ( ambient ) {
			float r = 0.4; // default
			float g = 0.4; // default
			float b = 0.4; // default
			float a = 1.0; // default
			ambient->QueryFloatAttribute( "R", &r );
			ambient->QueryFloatAttribute( "G", &g );
			ambient->QueryFloatAttribute( "B", &b );
			ambient->QueryFloatAttribute( "A", &a );
			light.SetAmbient( r, g, b, a );
			}

		// diffuse
		TiXmlElement* diffuse = el->FirstChildElement( "diffuse" );
		if ( diffuse ) {
			float r = 0.4; // default
			float g = 0.4; // default
			float b = 0.4; // default
			float a = 1.0; // default
			diffuse->QueryFloatAttribute( "R", &r );
			diffuse->QueryFloatAttribute( "G", &g );
			diffuse->QueryFloatAttribute( "B", &b );
			diffuse->QueryFloatAttribute( "A", &a );
			light.SetDiffuse( r, g, b, a );
			}

		// specular
		TiXmlElement* specular = el->FirstChildElement( "specular" );
		if ( specular ) {
			float r = 0.4; // default
			float g = 0.4; // default
			float b = 0.4; // default
			float a = 1.0; // default
			specular->QueryFloatAttribute( "R", &r );
			specular->QueryFloatAttribute( "G", &g );
			specular->QueryFloatAttribute( "B", &b );
			specular->QueryFloatAttribute( "A", &a );
			light.SetSpecular( r, g, b, a );
			}

		// position
		TiXmlElement* position = el->FirstChildElement( "position" );
		if ( position ) {
			float x = 1.0; // default
			float y = 1.0; // default
			float z = 1.0; // default
			int p = 0; // default
			position->QueryFloatAttribute( "X", &x );
			position->QueryFloatAttribute( "Y", &y );
			position->QueryFloatAttribute( "Z", &z );
			position->QueryIntAttribute( "positional", &p );
			light.SetPosition( x, y, z );
			light.SetDirectional( p );
			}

		// spot direction
		TiXmlElement* spot_direction = el->FirstChildElement( "spot_direction" );
		if ( spot_direction ) {
			float x = -1.0; // default
			float y = -1.0; // default
			float z = -1.0; // default
			spot_direction->QueryFloatAttribute( "X", &x );
			spot_direction->QueryFloatAttribute( "Y", &y );
			spot_direction->QueryFloatAttribute( "Z", &z );
			light.SetDirection( x, y, z );
			}

		// spot_radius
		TiXmlElement* spot_radius = el->FirstChildElement( "spot_radius" );
		if ( spot_radius ) {
			float x = cast::lexical_cast< float, std::string >( spot_radius->GetText() );
			light.SetSpotRadius( x );
			}

		// spot_exponant
		TiXmlElement* spot_exponant = el->FirstChildElement( "spot_exponant" );
		if ( spot_exponant ) {
			float x = cast::lexical_cast< float, std::string >( spot_exponant->GetText() );
			light.SetSpotExponant( x );
			}

		// constant_attenuation
		TiXmlElement* constant_attenuation = el->FirstChildElement( "constant_attenuation" );
		if ( constant_attenuation ) {
			float x = cast::lexical_cast< float, std::string >( constant_attenuation->GetText() );
			light.SetConstantAttenuation( x );
			}

		// linear_attenuation
		TiXmlElement* linear_attenuation = el->FirstChildElement( "linear_attenuation" );
		if ( linear_attenuation ) {
			float x = cast::lexical_cast< float, std::string >( linear_attenuation->GetText() );
			light.SetLinearAttenuation( x );
			}

		// quadratic_attenuation
		TiXmlElement* quadratic_attenuation = el->FirstChildElement( "quadratic_attenuation" );
		if ( quadratic_attenuation ) {
			float x = cast::lexical_cast< float, std::string >( quadratic_attenuation->GetText() );
			light.SetQuadraticAttenuation( x );
			}


		// create a data packet and make an entry
		// NOTE: no raw data, just use a clonable object.
		name_counter++;
		lights.insert(   std::make_pair( name_counter, MapEntry<Light*,Light>( NULL, light ) )   );

		// scan for aliases
		if ( !el->NoChildren() ) {  ScanLightForAliases( el, name_counter ); }

		}
	}



void ResourceMgr::ScanConfigForMaterials( TiXmlDocument& doc ) {

	static int name_counter = 0;

	// find LIGHTS tag
	TiXmlNode* xmlmats = doc.FirstChild( "materials" );
	if ( !xmlmats ) {
		LogMgr::Write( LOGMSG_ERROR, "No MATERIALS tag found in config file '%s'. Materials will not load!", doc.Value() );
		return;
		}

	// foreach light
	for ( TiXmlElement* el = xmlmats->FirstChildElement("material"); el; el = el->NextSiblingElement("material") ) {

// 		<material>
// 			<alias>default</alias> <!-- NOTE: this is different than the other aliases -->
// 			<ambient R="0.4" G="0.4" B="0.4" A="1" />
// 			<diffuse R="1" G="1" B="1" A="1" />
// 			<specular R="1" G="1" B="1" A="1" />
// 			<shine>90</shine>
// 			<emission R="0" G="0" B="0" A="0" />
// 		</material>

		// create the stub
		Material m = Material();

		// ambient
		TiXmlElement* ambient = el->FirstChildElement( "ambient" );
		if ( ambient ) {
			float r = 0.4; // default
			float g = 0.4; // default
			float b = 0.4; // default
			float a = 1.0; // default
			ambient->QueryFloatAttribute( "R", &r );
			ambient->QueryFloatAttribute( "G", &g );
			ambient->QueryFloatAttribute( "B", &b );
			ambient->QueryFloatAttribute( "A", &a );
			m.SetAmbient( r, g, b, a );
			}

		// diffuse
		TiXmlElement* diffuse = el->FirstChildElement( "diffuse" );
		if ( diffuse ) {
			float r = 0.4; // default
			float g = 0.4; // default
			float b = 0.4; // default
			float a = 1.0; // default
			diffuse->QueryFloatAttribute( "R", &r );
			diffuse->QueryFloatAttribute( "G", &g );
			diffuse->QueryFloatAttribute( "B", &b );
			diffuse->QueryFloatAttribute( "A", &a );
			m.SetDiffuse( r, g, b, a );
			}

		// specular
		TiXmlElement* specular = el->FirstChildElement( "specular" );
		if ( specular ) {
			float r = 0.4; // default
			float g = 0.4; // default
			float b = 0.4; // default
			float a = 1.0; // default
			specular->QueryFloatAttribute( "R", &r );
			specular->QueryFloatAttribute( "G", &g );
			specular->QueryFloatAttribute( "B", &b );
			specular->QueryFloatAttribute( "A", &a );
			m.SetSpecular( r, g, b, a );
			}

		// emission
		TiXmlElement* emission = el->FirstChildElement( "emission" );
		if ( emission ) {
			float r = 0.4; // default
			float g = 0.4; // default
			float b = 0.4; // default
			float a = 1.0; // default
			emission->QueryFloatAttribute( "R", &r );
			emission->QueryFloatAttribute( "G", &g );
			emission->QueryFloatAttribute( "B", &b );
			emission->QueryFloatAttribute( "A", &a );
			m.SetEmission( r, g, b, a );
			}

		// shine
		TiXmlElement* shine = el->FirstChildElement( "shine" );
		if ( shine ) {
			float x = cast::lexical_cast< float, std::string >( shine->GetText() );
			m.SetShine( x );
			}

		// create a data packet and make an entry
		// NOTE: no raw data, just use a clonable Material object.
		name_counter++;
		materials.insert(   std::make_pair( name_counter, MapEntry<Material*,Material>( NULL, m ) )   );

		// scan for aliases
		if ( !el->NoChildren() ) {  ScanMaterialForAliases( el, name_counter ); }

		}
	}



void ResourceMgr::ScanConfigForMusic( TiXmlDocument& doc ) {

	static int name_counter = 0;

	// find MUSIC tag
	TiXmlNode* xmltracks = doc.FirstChild( "music" );
	if ( !xmltracks ) {
		LogMgr::Write( LOGMSG_ERROR, "No MUSIC tag found in config file '%s'. Music will not load!", doc.Value() );
		return;
		}

	// dissect each entry
	for ( TiXmlElement* el = xmltracks->FirstChildElement(); el; el = el->NextSiblingElement() ) {

		// check for filename:
		std::string file(  el->Attribute("file") ? el->Attribute( "file" ) : "" );
		if ( file.empty() ) {
			LogMgr::Write( LOGMSG_WARNING, "No proper track filename found in config file '%s', Row #%d. Track will not load", doc.Value(), el->Row() );
			continue;
			}

		// create a data packet and make an entry
		name_counter++;
		TrackData data = { file };
		tracks.insert(   std::make_pair( name_counter, MapEntry<Mix_Music*,TrackData>( NULL, data ) )   );

		// scan for aliases
		if ( !el->NoChildren() ) {  ScanSongForAliases( el, name_counter ); }

		}
	}


void ResourceMgr::ScanConfigForFonts( TiXmlDocument& doc ) {

	static int name_counter = 0;

	// find FONTS tag
	TiXmlNode* xmlfonts = doc.FirstChild( "fonts" );
	if ( !xmlfonts ) {
		LogMgr::Write( LOGMSG_ERROR, "No FONTS tag found in config file '%s'. Fonts will not load!", doc.Value() );
		return;
		}

	// dissect each entry
	for ( TiXmlElement* el = xmlfonts->FirstChildElement(); el; el = el->NextSiblingElement() ) {

		// check for filename:
		std::string file(  el->Attribute("file") ? el->Attribute( "file" ) : "" );
		if ( file.empty() ) {
			LogMgr::Write( LOGMSG_WARNING, "No proper font filename found in config file '%s', Row #%d. Font will not load", doc.Value(), el->Row() );
			continue;
			}

		// check for size
		int size = 12;
		if ( el->QueryIntAttribute( "size", &size ) ) {
			LogMgr::Write( LOGMSG_WARNING, "No proper font size found in config file '%s', Row #%d. Inserting default [12].", el->GetDocument()->Value(), el->Row() );
			size = 12;
			}

		// check for advance
		std::string advance(  el->Attribute( "advance" ) ? el->Attribute( "advance" ) : ""  );

		// create a data packet and make an entry
		name_counter++;
		FontData data = { file, size, advance };
		fonts.insert(   std::make_pair( name_counter, MapEntry<FontImpl*,FontData>( NULL, data ) )   );

		// scan for aliases
		if ( !el->NoChildren() ) {  ScanFontForAliases( el, name_counter ); }

		}
	}



void ResourceMgr::ScanConfigForSounds( TiXmlDocument& doc ) {

	static int name_counter = 0;

	// find SOUNDFX tag
	TiXmlNode* xmlsounds = doc.FirstChild( "soundfx" );
	if ( !xmlsounds ) {
		LogMgr::Write( LOGMSG_ERROR, "No SOUNDFX tag found in config file '%s'. Sound effects will not load!", doc.Value() );
		return;
		}

	// dissect each entry
	for ( TiXmlElement* el = xmlsounds->FirstChildElement(); el; el = el->NextSiblingElement() ) {

		// check for filename:
		std::string file(  el->Attribute("file") ? el->Attribute( "file" ) : "" );
		if ( file.empty() ) {
			LogMgr::Write( LOGMSG_WARNING, "No proper sound filename found in config file '%s', Row #%d. Sound will not load", doc.Value(), el->Row() );
			continue;
			}

		// create a data packet and make an entry
		name_counter++;
		SoundData data = { file };
		sounds.insert(   std::make_pair( name_counter, MapEntry<Mix_Chunk*,SoundData>( NULL, data ) )   );

		// scan for aliases
		if ( !el->NoChildren() ) {  ScanSoundForAliases( el, name_counter ); }

		}
	}


void ResourceMgr::ScanConfigForTextures( TiXmlDocument& doc ) {

	// find TEXTURES tag
	TiXmlNode* xmltextures = doc.FirstChild( "textures" );
	if ( !xmltextures ) {
		LogMgr::Write( LOGMSG_ERROR, "No TEXTURES tag found in config file '%s'. Textures will not load!", doc.Value() );
		return;
		}

	// dissect each texture entry
	for ( TiXmlElement* el = xmltextures->FirstChildElement(); el; el = el->NextSiblingElement() ) {

		// check for handle;
		std::string handle(  el->Attribute("handle") ? el->Attribute("handle") : ""  );
		if ( handle.empty() ) {
			LogMgr::Write( LOGMSG_ERROR, "No proper texture handle found in config file '%s', Row #%d.", doc.Value(), el->Row() );
			continue;
			}

		// check if we already have that handle:
		if ( textures.find( handle ) != textures.end() ) {
			LogMgr::Write( LOGMSG_WARNING, "Duplicate texture handle in config file '%s', Row #%d. Resource ignored.", doc.Value(), el->Row() );
			}

		// check for filename:
		std::string file(  el->Attribute( "file" ) ? el->Attribute( "file" ) : ""  );
		if ( file.empty() ) {
			LogMgr::Write( LOGMSG_WARNING, "No proper texture filename found in config file '%s', Row #%d. Texture will not load", doc.Value(), el->Row() );
			continue;
			}

		textures.insert(  std::make_pair( handle, MapEntry<Texture,std::string>( Texture(), file ) )  );

		if ( !el->NoChildren() ) {  ScanTextureForAreas( el, handle ); }
		else { // create a single subtexture anyway
			SubtextureData data = { handle, AreaRect<int>(0,0,0,0) };
			subtexture_info.insert(   make_pair( handle, data )   );
			}
		}

	}

void ResourceMgr::ScanTextureForAreas( TiXmlElement* texture_el, std::string parent_name ) {
	// get the handle and area coords data now
	for ( TiXmlElement* el2 = texture_el->FirstChildElement(); el2; el2 = el2->NextSiblingElement() ) {

		// check for handle;
		std::string handle(  el2->Attribute("handle") ? el2->Attribute("handle") : ""  );
		if ( handle.empty() ) {
			LogMgr::Write( LOGMSG_ERROR, "No proper texture area handle found in config file '%s', Row #%d.", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// check for duplicates
		if ( subtexture_info.find(handle) != subtexture_info.end() ) {
			LogMgr::Write( LOGMSG_ERROR, "Duplicate texture area handle found in config file '%s', Row #%d. Resource will be ignored", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// get coords
		int x1 = 0;
		int x2 = 0;
		int y1 = 0;
		int y2 = 0;

		if ( el2->QueryIntAttribute( "X1", &x1 ) ) {
			LogMgr::Write( LOGMSG_WARNING, "No proper texture area bounds for X1 value found in config file '%s', Row #%d. Inserting zero.", el2->GetDocument()->Value(), el2->Row() );
			}
		if ( el2->QueryIntAttribute( "X2", &x2 ) ) {
			LogMgr::Write( LOGMSG_WARNING, "No proper texture area bounds for X2 value found in config file '%s', Row #%d. Inserting zero.", el2->GetDocument()->Value(), el2->Row() );
			}
		if ( el2->QueryIntAttribute( "Y1", &y1 ) ) {
			LogMgr::Write( LOGMSG_WARNING, "No proper texture area bounds for Y1 value found in config file '%s', Row #%d. Inserting zero.", el2->GetDocument()->Value(), el2->Row() );
			}
		if ( el2->QueryIntAttribute( "Y2", &y2 ) ) {
			LogMgr::Write( LOGMSG_WARNING, "No proper texture area bounds for Y2 value found in config file '%s', Row #%d. Inserting zero.", el2->GetDocument()->Value(), el2->Row() );
			}

		// create info record
		SubtextureData data = { parent_name, AreaRect<int>(x1,y1,x2,y2) };
		subtexture_info.insert(   std::make_pair( handle, data )   );

		}
	}

void ResourceMgr::ScanFontForAliases( TiXmlElement* font_el, int font_name ) {
	for ( TiXmlElement* el2 = font_el->FirstChildElement(); el2; el2 = el2->NextSiblingElement() ) {

		// check for handle;
		std::string handle(  el2->Attribute("handle") ? el2->Attribute("handle") : 0  );
		if ( handle.empty() ) {
			LogMgr::Write( LOGMSG_ERROR, "No proper font alias handle found in config file '%s', Row #%d.", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// check for duplicates
		if ( font_aliases.find(handle) != font_aliases.end() ) {
			LogMgr::Write( LOGMSG_ERROR, "Duplicate font alias handle found in config file '%s', Row #%d. Resource will be ignored", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// match alias -> font
		font_aliases.insert(   std::make_pair( handle, font_name )   );

		}
	}

void ResourceMgr::ScanSoundForAliases( TiXmlElement* sound_el, int sound_name ) {
	for ( TiXmlElement* el2 = sound_el->FirstChildElement(); el2; el2 = el2->NextSiblingElement() ) {

		// check for handle;
		std::string handle(  el2->Attribute("handle") ? el2->Attribute("handle") : 0  );
		if ( handle.empty() ) {
			LogMgr::Write( LOGMSG_ERROR, "No proper sound alias handle found in config file '%s', Row #%d.", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// check for duplicates
		if ( sound_aliases.find(handle) != sound_aliases.end() ) {
			LogMgr::Write( LOGMSG_ERROR, "Duplicate sound alias handle found in config file '%s', Row #%d. Resource will be ignored", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// match alias -> sound
		sound_aliases.insert(  std::make_pair( handle, sound_name )   );

		}
	}


void ResourceMgr::ScanSongForAliases( TiXmlElement* song_el, int song_name ) {
	for ( TiXmlElement* el2 = song_el->FirstChildElement(); el2; el2 = el2->NextSiblingElement() ) {

		// check for handle;
		std::string handle(  el2->Attribute("handle") ? el2->Attribute("handle") : 0  );
		if ( handle.empty() ) {
			LogMgr::Write( LOGMSG_ERROR, "No proper track alias handle found in config file '%s', Row #%d.", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// check for duplicates
		if ( track_aliases.find(handle) != track_aliases.end() ) {
			LogMgr::Write( LOGMSG_ERROR, "Duplicate track alias handle found in config file '%s', Row #%d. Resource will be ignored", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// match alias -> sound
		track_aliases.insert(  std::make_pair( handle, song_name )   );

		}
	}



void ResourceMgr::ScanMaterialForAliases( TiXmlElement* mat_el, int mat_name ) {

	// cycle through all elements to look for aliases
	for ( TiXmlElement* el2 = mat_el->FirstChildElement("alias"); el2; el2 = el2->NextSiblingElement("alias") ) {

		// check for alias data
		std::string alias( el2->GetText() );
		if ( alias.empty() ) {
			LogMgr::Write( LOGMSG_ERROR, "Empty material alias found in config file '%s', Row #%d.", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// check for duplicates
		if ( material_aliases.find(alias) != material_aliases.end() ) {
			LogMgr::Write( LOGMSG_ERROR, "Duplicate material alias found in config file '%s', Row #%d. Resource will be ignored", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// match alias -> material
		material_aliases.insert(  std::make_pair( alias, mat_name )   );

		}

	}


void ResourceMgr::ScanLightForAliases( TiXmlElement* light_el, int light_name ) {

	// cycle through all elements to look for aliases
	for ( TiXmlElement* el2 = light_el->FirstChildElement("alias"); el2; el2 = el2->NextSiblingElement("alias") ) {

		// check for alias data
		std::string alias( el2->GetText() );
		if ( alias.empty() ) {
			LogMgr::Write( LOGMSG_ERROR, "Empty light alias found in config file '%s', Row #%d.", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// check for duplicates
		if ( light_aliases.find(alias) != light_aliases.end() ) {
			LogMgr::Write( LOGMSG_ERROR, "Duplicate light alias found in config file '%s', Row #%d. Resource will be ignored", el2->GetDocument()->Value(), el2->Row() );
			continue;
			}

		// match alias -> material
		light_aliases.insert(  std::make_pair( alias, light_name )   );

		}

	}

/*///////////////////////////////////////////////////////////////////////////

===========\/============  DEBUG FUNCTIONS  ================\/===============

///////////////////////////////////////////////////////////////////////////*/





void ResourceMgr::DebugReportLoadedResources() {

	LogMgr::Write( LOGMSG_DEBUG, "Resource Manager - Loaded Resources Report:" );

	int gotchas = 0;

	// TEXTURES --------------------------------------------------------------
	for (textures_iter iter = textures.begin(); iter != textures.end(); iter++) {
		if ( iter->second.resource.IsValid() ) {
			LogMgr::Write( LOGMSG_DEBUG, "   [TEXTURE] \"%s\" checked out by:", iter->first.c_str() );
			for( MapEntry<Texture,std::string>::reserves_iter subiter = iter->second.reserves.begin(); subiter != iter->second.reserves.end(); subiter++) {
				LogMgr::Write( LOGMSG_DEBUG, "       \"%s\" [holds: %d]", subiter->first.c_str(), subiter->second );
				}
			gotchas++;
			}
		}

	// FONTS ------------------------------------------------------------------
	for (fonts_iter iter = fonts.begin(); iter != fonts.end(); iter++) {
		if ( iter->second.resource ) {
			LogMgr::Write( LOGMSG_DEBUG, "   [FONT] #%d (%s) checked out by:", iter->first, iter->second.data.file.c_str() );
			for( MapEntry<Font,FontData>::reserves_iter subiter = iter->second.reserves.begin(); subiter != iter->second.reserves.end(); subiter++) {
				LogMgr::Write( LOGMSG_DEBUG, "       \"%s\" [holds: %d]", subiter->first.c_str(), subiter->second );
				}
			gotchas++;
			}
		}

	// SOUNDS ------------------------------------------------------------------
	for (sounds_iter iter = sounds.begin(); iter != sounds.end(); iter++) {
		if ( iter->second.resource ) {
			LogMgr::Write( LOGMSG_DEBUG, "   [SOUND] #%d (%s) checked out by:", iter->first, iter->second.data.file.c_str() );
			for( MapEntry<Mix_Chunk*,SoundData>::reserves_iter subiter = iter->second.reserves.begin(); subiter != iter->second.reserves.end(); subiter++) {
				LogMgr::Write( LOGMSG_DEBUG, "       \"%s\" [holds: %d]", subiter->first.c_str(), subiter->second );
				}
			gotchas++;
			}
		}

	// TRACKS ------------------------------------------------------------------
	for (tracks_iter iter = tracks.begin(); iter != tracks.end(); iter++) {
		if ( iter->second.resource ) {
			LogMgr::Write( LOGMSG_DEBUG, "   [MUSIC TRACK] #%d (%s) checked out by:", iter->first, iter->second.data.file.c_str() );
			for( MapEntry<Mix_Music*,TrackData>::reserves_iter subiter = iter->second.reserves.begin(); subiter != iter->second.reserves.end(); subiter++) {
				LogMgr::Write( LOGMSG_DEBUG, "       \"%s\" [holds: %d]", subiter->first.c_str(), subiter->second );
				}
			gotchas++;
			}
		}

	if (!gotchas) {
		LogMgr::Write( LOGMSG_DEBUG, "       - No Resources Checked Out (Woohoo!) -" );
		}
	}





void ResourceMgr::DebugReportRegisteredClasses() {
	LogMgr::Write( LOGMSG_DEBUG, "Resource Manager - Client-Registry List:" );

	int gotchas = 0;

	for (registrations_iter iter = class_registry.begin(); iter != class_registry.end(); iter++) {
		LogMgr::Write( LOGMSG_DEBUG, "   [holds: %d] \"%s\"", iter->second.holds, iter->first.c_str() );
		gotchas++;
		}

	if (!gotchas) {
		LogMgr::Write( LOGMSG_DEBUG, "       - No classes registered as resource clients -" );
		}
	}




} // end namespace LGC






