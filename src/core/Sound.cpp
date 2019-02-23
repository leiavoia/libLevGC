#include "Sound.h"

#include "../utilities/LogManager.h"
#include <math.h>
#include "extra_math.h"
#include "../algo/collision/Vector.h"




namespace LGC {




int Sound::player_x = 0;
int Sound::player_y = 0;



void Sound::Play( int volume, int loops, int timeout, int fade_in  ) {
	if (!sound) { return; }

	Mix_VolumeChunk(sound, volume);

	if (fade_in > 0) {
		channel = Mix_FadeInChannelTimed(-1, sound, loops, fade_in, timeout);
		}
	else {
		channel = Mix_PlayChannelTimed(-1, sound, loops, timeout);
		}
	}

void Sound::PlayAt( float x, float y, int volume, int loops, int timeout, int fade_in  ) {
	Vector delta(x - (float)player_x, y - (float)player_y);
	Vector compare(0,100);
	int dist = (int)delta.Length();
	if (dist > 1600) { return; } // out of range
	float angle = delta.Angle(compare);

	Play( volume, loops, timeout, fade_in );
	SetLocation( dist, (int)RadToDeg(angle)  );
	}


void Sound::Stop( float fade_out ) {
	if (!sound || channel == -1) { return; }
	Mix_HaltChannel(channel);
	channel = -1;
	}

void Sound::Pause() {
	if (!sound || channel == -1) { return; }
	Mix_Pause(channel);
	}

void Sound::Unpause() {
	if (!sound || channel == -1) { return; }
	Mix_Resume(channel);
	}

bool Sound::IsPaused() {
	if (!sound || channel == -1) { return false; }
	return Mix_Paused(channel);
	}

bool Sound::IsPlaying() {
	if (!sound || channel == -1) { return false; }
	return Mix_Playing(channel);
	}

void Sound::SetLocation( int dist, int angle ) {
	if (!sound || channel == -1) { return; }
	if (angle < 0) { angle += 360; }
	if ( !Mix_SetPosition(channel, (Sint16)angle, Uint8(dist/6) ) ) {
		LogMgr::Write( LOGMSG_ERROR, "Problem with Position setting");
		}
	//LogMgr::Write( LOGMSG_DEBUG, "Dist: [%d] Volume: [%d], Angle: %d", dist, dist/6, angle);
	}




void Sound::UpdatePlayerLocation(float x, float y) {
	player_x = (int)x;
	player_y = (int)y;
	}

void Sound::SetMasterVolume( float x ) {
	int num_channels = Mix_AllocateChannels(-1);
	int vol = int(128.0 * x);
	for (int i=0; i < num_channels; i++) {
		Mix_Volume( i, vol );
		}
	}

void Sound::PauseAllChannels( bool x ) {
	int num_channels = Mix_AllocateChannels(-1);
	for ( int i = 0; i < num_channels; i++ ) {
		if ( x ) { Mix_Pause(i); } else { Mix_Resume(i); }
		}
	}


} // end namespace LGC



//



