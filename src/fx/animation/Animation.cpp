
#include "Animation.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
#include "../../utilities/LogManager.h"

#include "../../core/GameSpace.h"
#include "../../core/ResourceMgr.h"

#include "../../utilities/TinyXML/tinyxml.h"

namespace LGC {



Animation::Animation( float x, float y ): PhysicalObject(x,y) {
	repeat = false;
	reps = 0; // -1 = forever
	reps_done = 0;
	runtime = 0;
	frame_num = 0;
	}



Animation::~Animation() {
	}



bool Animation::BackPoll(float timedelta, int num_frames, std::vector<float> &times) {

	// add interval time to running time
	runtime += timedelta;

	// check reps - all done?
	if ( reps_done > reps && reps != -1) { return false; }

	// fast forward to the correct frame:
	while ( runtime > times[frame_num] ) {
		if ( frame_num < num_frames ) { frame_num++; } else { break; }
		}
		
	// check max num frames overflow (starting next rep)
	if ( frame_num >= num_frames ) {
		// recycle if repeat is on
		if (repeat) {
			reps_done++;
			frame_num = 0;
			runtime = 0;
			}
		// otherwise we're all done
		else { 
			frame_num = num_frames-1;
			return false; 
			}
		}

	return true;
	}

	
	
	
bool Animation::InterpBackPoll(
	float timedelta, 
	int num_frames, 
	std::vector<float> &times,
	int& frame_one,
	float& frame_one_percent,
	int& frame_two,
	float& frame_two_percent
	) {


		
	// add interval time to running time
	runtime += timedelta;

	// check reps - all done?
	if ( reps_done > reps && reps != -1) { return false; }

	// fast forward to the correct frame:
	while ( runtime > times[frame_num] ) {
		if ( frame_num < num_frames ) { frame_num++; } else { break; }
		}

	// check max num frames overflow (starting next rep)
	if ( frame_num >= num_frames ) {
		// recycle if repeat is on
		if (repeat) {
			reps_done++;
			frame_num = 0;
			runtime = 0;
			}
		// otherwise we're all done
		else { 
			frame_num = num_frames-1;
			return false; 
			}
		}
		
	
//  	frame_one = frame_num;
//  	frame_two = frame_num;
//  	frame_one_percent = 0.5;
//  	frame_two_percent = 0.5;
	
	// frame_num should now be at the correct time mark.
	frame_one = frame_num;
	frame_two = frame_num+1;
	// past the end?
	if (frame_one < 0) { frame_one = 0; }
	if (frame_two >= num_frames) { 
		frame_two = frame_one; 
// 		frame_two = 0; 
// 		if (repeat && reps_done == reps && reps != -1) {
// 			frame_two = frame_one; 
// 			}
		}
		

// 	LogMgr::Write(LOGMSG_DEBUG, "frame_one [%d] and frame_two [%d] of [%d] frames",
// 		frame_one,
// 		frame_two,
// 		num_frames
// 		);
		
					
// 	float first_time = times[frame_one];
// 	float last_time = times[frame_two];
// 	
// 	float first_time_int = (first_time - runtime);
// 	float diff = second_time - first_time;
	
	frame_one_percent = 0.5;
	frame_two_percent = 0.5;
	
// 	float second_time_int = (second_time - runtime - (second_time - first_time));
// 	if (second_time_int < 0) { 
// 		second_time_int = second_time + (times.back() - runtime);
// 		}
// 		
// 	float total = first_time_int + second_time_int;
// 	frame_one_percent = first_time_int / total;
// 	frame_two_percent = second_time_int / total;
	

	
	return true;
	}








bool Animation::LoadConfigFile(
	std::string filename, 
	std::vector<Texture> &frames, 
	std::vector<float> &times, 
	std::string classname
	) {

	// parse the XML file
	TiXmlDocument doc( filename );
	if ( !doc.LoadFile() ) {
		printf( "Could not load open config file '%s'. Error='%s'. Animation not loaded.\n", filename.c_str(), doc.ErrorDesc() );
		return false;
		}

	// check for any other loading errors
	if ( doc.Error() ) {
		printf( "Problem loading '%s'. Error='%s'. Animation not loaded.\n", filename.c_str(), doc.ErrorDesc() );
		return false;
		}

	// find ANIMATION tag
	TiXmlNode* anim = doc.FirstChild( "ANIMATION" );
	if ( !anim ) {
		printf( "No ANIMATION tag found in config file '%s'. Animation not loaded.\n", filename.c_str() );
		return false;
		}

	// dissect each frame in the animation
	TiXmlElement* el = NULL;
	float current_total = 0; // total time
	for ( el = anim->FirstChildElement(); el; el = el->NextSiblingElement() ) {

		// check for frame time;
		int time = 0;
		if ( el->QueryIntAttribute( "TIME", &time ) ) {
			printf( "No proper time value found in config file '%s'. Inserting zero.\n", filename.c_str() );
			}
		// insert into array:
		current_total += (float)time / 1000.0;
		times.push_back( current_total ); // switch to floats here


		// check for handle name:
		std::string handle(  el->Attribute( "HANDLE" ) ? el->Attribute( "HANDLE" ) : ""  );
		if ( handle.empty() ) {
			printf( "No proper handle found in config file '%s'. Inserting NULL frame.\n", filename.c_str() );
			}
		// load texture and insert into array
		frames.push_back(  ResourceMgr::Inst()->GetTexture( handle, classname )  );
		}

	return true; // success! 


// 	std::ifstream f( filename.c_str() );
//
// 	if (f.bad() || !f.is_open()) {
// 		LogMgr::Write(LOGMSG_ERROR, "Animation::LoadFrameFile(std::string filename): error opening file (check existance and spelling of file name and config file)");
// 		return false;
// 		}
// 
// 
// 	std::string s; // buffer
// 	std::vector<std::string> files; // list of files (temporary)
// 	int current_total = 0; // total time to add the next time segment to
// 
// 
// 	// loop through the file until end is reached
// 	while( !std::getline(f,s).eof() ) {
// 		// check for comments
// 		if (s[0] == '#') {
// 			LogMgr::Write(LOGMSG_DEBUG, "Animation::LoadConfigFile: found # comment");
// 			continue;
// 			}
// 		// check for blank lines
// 		else if (!s[0]) {
// 			LogMgr::Write(LOGMSG_DEBUG, "Animation::LoadConfigFile: found blank line");
// 			continue;
// 			}
// 		// TODO: CHECK FOR REPEAT, ETC
// 		// otherwise assume it's a frame time
// 		else {
// 			// put the frame time in the times vector
// 			current_total += atoi( s.c_str() );
// 			times.push_back( current_total );
// 			LogMgr::Write(LOGMSG_DEBUG, "Animation::LoadConfigFile: total is now: %d", current_total);
// 			LogMgr::Write(LOGMSG_DEBUG, "Animation::LoadConfigFile: found frame time: %s", s.c_str());
// 			// get the next line which should be the file to load
// 			std::getline(f,s);
// 			files.push_back(s);
// 			LogMgr::Write(LOGMSG_DEBUG, "Animation::LoadConfigFile: found file name: %s", s.c_str());
// 			}
// 		}

// 	// now that the config files are loaded, open the actual files to new OpenGL Textures
// 	for (unsigned int x=0; x < files.size(); x++) {
// 		LogMgr::Write(LOGMSG_DEBUG, "Loading surface: %s\n\t for time interval [%d]", files[x].c_str(), times[x]);
// 		frames.push_back(   LoadTexture( files[x].c_str() )   );
// 		}

//	return true;
	}



// 
// bool Animation::LoadConfigFile(std::string filename, std::vector<int> &frames, std::vector<int> &times, TextureSheet* &sheet) {
// 
// 	// parse the XML file
// 	TiXmlDocument doc( filename );
// 	if ( !doc.LoadFile() ) {
// 		LogMgr::Write(LOGMSG_ERROR, "Could not load open config file '%s'. Error='%s'. Animation not loaded.\n", filename.c_str(), doc.ErrorDesc() );
// 		return false;
// 		}
// 
// 	// check for any other loading errors
// 	if ( doc.Error() ) {
// 		LogMgr::Write(LOGMSG_ERROR, "Problem loading '%s'. Error='%s'. Animation not loaded.\n", filename.c_str(), doc.ErrorDesc() );
// 		return false;
// 		}
// 
// 	// find ANIMATION tag
// 	TiXmlElement* anim = doc.FirstChildElement( "ANIMATION" );
// 	if ( !anim ) {
// 		LogMgr::Write(LOGMSG_ERROR, "No ANIMATION tag found in config file '%s'. Animation not loaded.\n", filename.c_str() );
// 		return false;
// 		}
//  
// 	// find the handle of the resource (texture sheet) this animation needs
// 	const char* sheetname = anim->Attribute( "HANDLE" );
// 	if (!sheetname) {
// 		LogMgr::Write(LOGMSG_WARNING, "No proper texture sheet handle found in config file '%s'. Inserting NULL frame.\n", filename.c_str() );
// 		}
// 	sheet = ResourceMgr::Inst()->GetTextureSheet( sheetname, "Animation" );
// 	// /\  Use "Animation" as the reservers name to stand for all animations.
// 	// Not smart, but a workaround is clumsier :-(
// 	// Most animations will not share resource files anyway :-)
// 
// 	if (!sheet) {
// 		LogMgr::Write(LOGMSG_ERROR, "Error loading texture sheet '%s'", sheetname );
// 		return false;
// 		}
// 
// 
// 	// dissect each frame in the animation
// 	TiXmlElement* el = NULL;
// 	int current_total = 0; // total time
// 	for ( el = anim->FirstChildElement(); el; el = el->NextSiblingElement() ) {
// 
// 		// check for frame time;
// 		int time = 0;
// 		if ( el->QueryIntAttribute( "TIME", &time ) ) {
// 			LogMgr::Write(LOGMSG_WARNING, "No proper time value found in config file '%s'. Inserting zero.\n", filename.c_str() );
// 			}
// 		// insert into array:
// 		current_total += time;
// 		times.push_back( current_total );
// 
// 
// 		// check for filename:
// 		const char* areahandle = el->Attribute( "HANDLE" );
// 		if (!areahandle) {
// 			LogMgr::Write(LOGMSG_WARNING, "No proper resource handle found in config file '%s'. Inserting NULL area.\n", filename.c_str() );
// 			}
// 		// find handle frame index and insert into array
// 		frames.push_back(  sheet->GetIndexByHandle( areahandle )  );
// 		}
// 
// 	return true; // success!
// 	}







// 	void Animation::DrawFrame(Texture* t) {
// 		if (!t) { return; }
// 
// 			// FIXME: CENTERING
// 
// 
// 	// 		glBindTexture( GL_TEXTURE_2D, t->ID() );
// 	//
// 	// 		glPushMatrix();
// 	// 		glLoadIdentity();
// 	// 		glTranslatef(  float(xpos-gamespace->Xoff()) + 128.0/2, float(ypos-gamespace->Yoff()) + 128.0/2, 0  );
// 	// 		//glScalef( 0.2*frame_num+0.4, 0.2*frame_num+0.4, 0 );
// 	// 		glBegin(GL_QUADS);
// 	// 			glColor4f(1,1,1,1);
// 	// 			/* Bottom Left Of The Texture and Quad */
// 	// 			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( 64, -64, 0 );
// 	// 			/* Bottom Right Of The Texture and Quad */
// 	// 			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( 64, 64, 0 );
// 	// 			/* Top Right Of The Texture and Quad */
// 	// 			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -64, 64, 0 );
// 	// 			/* Top Left Of The Texture and Quad */
// 	// 			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -64, -64, 0 );
// 	// 		glEnd();
// 	//
// 	// 		glPopMatrix();
// 
// 		}



} // end namespace LGC




