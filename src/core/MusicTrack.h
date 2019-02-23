#ifndef MUSICTRACK_H
#define MUSICTRACK_H

#include <string>

#include <SDL/SDL_mixer.h>



// WARNING: HALT SOUND ON CHANNEL WHEN DELETING A SOUND CHUNK

// halt playback on all channels
// Mix_HaltChannel(-1);



namespace LGC {




class MusicTrack {
public:
	MusicTrack( std::string& name, Mix_Music* track ): name(name), track(track) {}
	MusicTrack(): name("NO_HANDLE"), track(NULL) {}
	~MusicTrack() {}
	MusicTrack( const MusicTrack& s ) {
		name = s.name;
		track = s.track;
		}	
	MusicTrack& operator = ( const MusicTrack& s ) {
		name = s.name;
		track = s.track;
		return *this;
		}
		
	inline std::string Handle() { return name; };
	
	inline bool Valid() { return (track != NULL); }

	void Play( int fade_in = 0 );
	void Stop( int fade_out = 0 );
	void Pause();
	void Unpause();	
	bool IsPaused();
	
	static void Volume( float x );
	
private:
	std::string name;	
	Mix_Music* track;
	};

	
} // end namespace LGC	



#endif






