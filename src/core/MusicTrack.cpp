#include "MusicTrack.h"

#include "../utilities/LogManager.h"


namespace LGC {




void MusicTrack::Play( int fade_in ) {
	if (!track) { return; }
	Mix_FadeInMusic(track, -1, fade_in);
	}
	
void MusicTrack::Stop( int fade_out ) {
	if (!track) { return; }
	Mix_FadeOutMusic( fade_out );
	}
	
void MusicTrack::Pause() {
	if (!track) { return; }
	Mix_PauseMusic();
	}
	
void MusicTrack::Unpause() {
	if (!track) { return; }
	Mix_ResumeMusic();
	}	
	
bool MusicTrack::IsPaused() {
	if (!track) { return true; }
	return Mix_PausedMusic();
	}

void MusicTrack::Volume( float x ) {
	Mix_VolumeMusic( int(128.0 * x) );
	}
	

	
} // end namespace LGC	


	
//












