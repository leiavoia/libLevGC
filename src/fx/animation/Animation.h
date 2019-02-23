
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "../../objects/PollingObject.h"
#include "../../objects/PhysicalObject.h"
#include "../../core/Texture.h"

#include <stdlib.h>





namespace LGC {

/** */
class Animation: public PhysicalObject, public PollingObject {
public:

	/** */
	virtual ~Animation();

	/** Returns 1 if frame was updated, 0 means "kill me" - animation has stopped*/
	virtual bool Poll() = 0;
	
	virtual void Draw() = 0;


protected:

	/** */
	Animation( float x, float y );

	/** Function used only by inherited animation widgets to crunch the actual poll calculation
	    NOTE: this version is for an array of Texture*'s */
	bool BackPoll(float timedelta, int num_frames/*std::vector<Texture*> &frames*/, std::vector<float> &times);

	bool InterpBackPoll (	
		float timedelta, 
		int num_frames, 
		std::vector<float> &times,
		int& frame_one,
		float& frame_one_percent,
		int& frame_two,
		float& frame_two_percent
		);
		
	/** Function used only by inherited animation widgets to crunch the actual poll calculation
	    NOTE: This version is for texture sheets. "Frames" is an array of texture area indexes */
	//bool BackPoll(int time, std::vector<int> &frames, std::vector<int> &times);

	/** Draws the frame onto the screen - single texture */
	//virtual void DrawFrame(Texture* t) {}

	/** Draws the frame onto the screen - texture sheet area */
	//virtual void DrawFrame(TextureSheet* sheet, int index) {}

	/** Returns FALSE on error, TRUE on clear.
	    NOTE: this version is for an array of Texture*'s */
	static bool LoadConfigFile(std::string filename, std::vector<Texture> &frames, std::vector<float> &times, std::string classname);

	/** Returns FALSE on error, TRUE on clear.
	    NOTE: This version is for TextureSheets. "Frames" is an array of texture area indexes */
	//static bool LoadConfigFile(std::string filename, std::vector<int> &frames, std::vector<int> &times, TextureSheet* &sheet );




	int frame_num;
	float runtime;
	bool repeat;
	int reps;
	int reps_done;




	};

	
} // end namespace LGC




#endif




