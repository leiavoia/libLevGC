#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#ifdef __WIN32__
#include <GL/glext.h>
#endif

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include <string>
#include <map>
#include <set>
#include <algorithm>
#include "Texture.h"
#include "Fonts.h"
#include "Sound.h"
#include "MusicTrack.h"
#include "../view/Light.h"
#include "../view/Material.h"

// #include <FTGL/FTGLTextureFont.h>
#include "../utilities/TinyXML/tinyxml.h"

/*
To Do:

*/




namespace LGC {






/*
This macro directs a class to pre-register as a resource borrower
with the resource manager when the program starts. ( C++ extratrickery )
To register a class as a borrower, after implementation of the class,
simply call:

  	REGISTER_CLASS_AS_RESOURCE_CLIENT( NameOfClass )

REQUIREMENTS:
Class must have two public static resource-negotiation functions called:

  	static void Class::ReserveResources()
	static void Class::DumpResources()

*/

#define REGISTER_CLASS_AS_RESOURCE_CLIENT(x) bool _register_##x##_fn() \
{\
ResourceMgr::Inst()->Register( #x, &x::ReserveResources, &x::DumpResources ); \
return true; \
}\
static bool _register_##x##_var = _register_##x##_fn();




/*
Here is another cute little macro for your convenience.
Now instead of typing

	ResourceMgr::Inst()->Whatever

you can type

	RM->Whatever

from anywhere you want!
*/

#define RM ResourceMgr::Inst()




// typedef for basic Load/Dump functions to register with the manager
typedef void (*register_func)(void);












class ResourceMgr {
public:


	static ResourceMgr* Inst(); // singleton instance

	~ResourceMgr() {}


	// BASIC INTERFACE

	/** Classes should call this function to register themselves */
	void Register( std::string class_name, register_func Load, register_func Dump );

	/** Call this to load the resources for a specific class or list thereof */
	bool ReserveResourcesFor( std::vector< std::string > classnames );
	bool ReserveResourcesFor( std::string classname );

	/** Call this to dump the resources for a specific class or list thereof */
	bool DumpResourcesFor( std::vector< std::string > classnames );
	bool DumpResourcesFor( std::string classname );

	/** Configures new resources from an XML file */
	bool LoadResourceXMLFile( const std::string& file );

	// TEXTURES
	Texture GetTexture( std::string handle, std::string id );
	void DumpTexture(const Texture& img, std::string id );

	// FONTS
	Font GetFont( std::string handle, std::string id );
	void DumpFont( Font& f, std::string id );

	// DEBUGGING
	void DebugReportLoadedResources();
	void DebugReportRegisteredClasses();

	// SOUND
	Sound GetSound( std::string handle, std::string id );
	void DumpSound( Sound& s, std::string id );

	// MUSIC
	MusicTrack GetTrack( std::string handle, std::string id );
	void DumpTrack( MusicTrack& t, std::string id );

	// MATERIAL
	Material GetMaterial( std::string handle, std::string id );
	void DumpMaterial( Material& x, std::string id );

	// LIGHT
	Light GetLight( std::string handle, std::string id );
	void DumpLight( Light& x, std::string id );

	// permanantly disables the loading of audio resources
	void DisableAudio() { use_audio = false; }



private:

	bool Init();

	ResourceMgr();

	std::string file_path; // to prepend to the files we are loading.

	// --------------------------------------------------------------------

	/** Holds data for classes that register to request resources.
	Each class must supply it's name and a function each for
	loading and dumping resources. A number of holds is also supplied
	to show how many times the resources have been requested for the
	entire class at once via ReserveResourcesFor( name_of_class ) */
	class RegistrationForm {
		public:
		RegistrationForm( register_func L, register_func D ) {
			Load = L;
			Dump = D;
			holds = 0;
			}
		RegistrationForm() {
			Load = NULL;
			Dump = NULL;
			holds = 0;
			}
		register_func Load;
		register_func Dump;
		int holds; // number of times an outsider has requested this class to load resources.
		};

	// --------------------------------------------------------------------

	/* This is an entry for registering any generic resource in an STL map*/
	template <class TYPE, class DATA> class MapEntry {
	public:
// 		MapEntry() {
// 			resource = TYPE();
// 			file = "";
// 			}
		MapEntry(TYPE t, DATA d) {
			resource = t;
			data = d;
			}

		// list of classes that have a hold on this resource
		typedef std::map< std::string, unsigned int >::iterator reserves_iter;
		std::map< std::string, unsigned int > reserves;
		TYPE resource;
		DATA data;

		void Hold( std::string& name ) {
			reserves_iter i = reserves.find(name);
			if ( i != reserves.end() ) { i->second++; }
			else { reserves.insert(  make_pair( name, 1 )  ); }
			}
		// returns true if the resource has no more holds, false if not
		bool Release( std::string& name ) {
			reserves_iter i = reserves.find(name);
			if ( i != reserves.end() && --(i->second) <= 0 )  { reserves.erase(i); }
			return (reserves.size() == 0);
			}
		bool NotLoaded() { return (reserves.size() == 0); }
		};

	// --------------------------------------------------------------------

	struct SubtextureData {
		std::string parent;
		AreaRect<int> area;
		};

	struct FontData {
		std::string file;
		int size;
		std::string advance;
		};

	struct SoundData {
		std::string file;
		};

	struct TrackData {
		std::string file;
		};



	// --------------------------------------------------------------------


	// the registry which maps class names to the load/dump functions
	std::map< std::string, RegistrationForm > 							class_registry;
	typedef std::map< std::string, RegistrationForm >::iterator 			registrations_iter;


	// HOW THESE WORK:
	// the main map for each type of resource stores the
	// quick-lookup indentifier for the resource (int or string),
	// the stub for the resource, and the data to load the resource.
	// When the resource is actually called for, the stub will be populated
	// with an actual object from the available data. Sometimes the "data"
	// can just be a clonable object.


	// maps texture names to texture structures
	std::map< std::string, MapEntry<Texture,std::string> > 				textures;
	typedef std::map< std::string, MapEntry<Texture,std::string> >::iterator	textures_iter;

	// a helper struct which maps subtextures to the texture
	std::map< std::string, SubtextureData > 							subtexture_info;
	typedef std::map< std::string, SubtextureData >::iterator				subtexture_info_iter;



	// maps font names to fonts
	std::map< int, MapEntry<FontImpl*,FontData> > 						fonts;
	typedef std::map< int, MapEntry<FontImpl*,FontData> >::iterator			fonts_iter;

	// a helper struct which maps font aliases to fonts
	std::map< std::string, int > 										font_aliases;
	typedef std::map< std::string, int >::iterator						font_aliases_iter;



	// maps sound names to sounds
	std::map< int, MapEntry<Mix_Chunk*,SoundData> > 						sounds;
	typedef std::map< int, MapEntry<Mix_Chunk*,SoundData> >::iterator		sounds_iter;

	// a helper struct which maps sound aliases to sounds
	std::map< std::string, int > 										sound_aliases;
	typedef std::map< std::string, int >::iterator						sound_aliases_iter;



	// maps song names to songs
	std::map< int, MapEntry<Mix_Music*,TrackData> > 						tracks;
	typedef std::map< int, MapEntry<Mix_Music*,TrackData> >::iterator		tracks_iter;

	// a helper struct which maps song data to the songs
	std::map< std::string, int > 										track_aliases;
	typedef std::map< std::string, int >::iterator						track_aliases_iter;



	// maps material names to materials
	std::map< int, MapEntry<Material*,Material> > 						materials;
	typedef std::map< int, MapEntry<Material*,Material> >::iterator			materials_iter;

	// a helper struct which maps material aliases to materials
	std::map< std::string, int > 										material_aliases;
	typedef std::map< std::string, int >::iterator						material_aliases_iter;



	// maps light names to lights
	std::map< int, MapEntry<Light*,Light> > 							lights;
	typedef std::map< int, MapEntry<Light*,Light> >::iterator				lights_iter;

	// a helper struct which maps light aliases to lights
	std::map< std::string, int > 										light_aliases;
	typedef std::map< std::string, int >::iterator						light_aliases_iter;



	// Function to load a texture from a file
	static Texture LoadTexture( std::string, std::string handle );


	// helper functions
	void ScanConfigForSounds( TiXmlDocument& doc );
	void ScanConfigForMusic( TiXmlDocument& doc );
	void ScanConfigForFonts( TiXmlDocument& doc );
	void ScanConfigForTextures( TiXmlDocument& doc );
	void ScanConfigForMaterials( TiXmlDocument& doc );
	void ScanConfigForLights( TiXmlDocument& doc );

	void ScanTextureForAreas( TiXmlElement* texture_el, std::string parent_name );
	void ScanFontForAliases( TiXmlElement* font_el, int font_name );
	void ScanSoundForAliases( TiXmlElement* sound_el, int sound_name );
	void ScanSongForAliases( TiXmlElement* song_el, int song_name );
	void ScanMaterialForAliases( TiXmlElement* mat_el, int mat_name );
	void ScanLightForAliases( TiXmlElement* light_el, int light_name );

	bool use_audio;
	};






} // end namespace LGC




#endif











