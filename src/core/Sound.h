#ifndef SOUND_H
#define SOUND_H

#include <string>

#include <SDL/SDL_mixer.h>



// WARNING: HALT SOUND ON CHANNEL WHEN DELETING A SOUND CHUNK

// halt playback on all channels
// Mix_HaltChannel(-1);

// WARNING:
// CODE DOES NOT HANDLE MULTIPLE OVERLAPPING PLAYING SOUNDS!!!!!!!!
// Stop, Pause, etc will not work on anything but the last one !!!!!!!!!!




namespace LGC {




class Sound {
public:
	Sound( std::string& name, Mix_Chunk* sound ): name(name), sound(sound), channel(-1) {}
	Sound(): name("NO_HANDLE"), sound(NULL), channel(-1) {}
	~Sound() {}
	Sound( const Sound& s ) {
		name = s.name;
		sound = s.sound;
		channel = s.channel;
		}
	Sound& operator = ( const Sound& s ) {
		name = s.name;
		sound = s.sound;
		channel = s.channel;
		return *this;
		}

	inline std::string Handle() { return name; };

	inline bool Valid() { return (sound != NULL); }

	// loops = 0 means "play once"
	// volume max is 128
	// timout (in ms) = -1 means "play forever"
	// fade_in = 0 means "no fade in"
	void Play(
		int volume = MIX_MAX_VOLUME,
		int loops = 0,
		int timeout = -1,
		int fade_in = 0
		);
	void PlayAt(
		float x,
		float y,
		int volume = MIX_MAX_VOLUME,
		int loops = 0,
		int timeout = -1,
		int fade_in = 0
		);
	void Stop( float fade_out = 0 );
	void Pause();
	void Unpause();
	bool IsPaused();
	bool IsPlaying();
	void SetLocation( int dist, int angle=0 );

	static void UpdatePlayerLocation(float x, float y);

	static void SetMasterVolume( float x );

	static void PauseAllChannels( bool x = true );

private:
	std::string name;
	Mix_Chunk* sound;
	int channel;

	static int player_x;
	static int player_y;
	};


} // end namespace LGC



#endif






