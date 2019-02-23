#ifndef LGC_CAMERA_H
#define LGC_CAMERA_H

namespace LGC {

class Camera {
			
			
	public: // ---------------------------------------------
			
	void LookAt( float time_delta = 0 ); // do GL transforms
		
	void SetLocation( float x, float y, float z );
	
	void SetTarget( float x, float y, float z, bool snap = false );
	
	void ChangeLens( float size, int screen_width, int screen_height );
	
	Camera( float x, float y, float z ) : xpos(x), ypos(y), zpos(z), servo1_angle(0), servo2_angle(0), snap(false) { }
		
	inline float X() { return xpos; }
	inline float Y() { return ypos; }
	inline float Z() { return zpos; }
	
	
	
	private: // --------------------------------------------
			
	float xpos, ypos, zpos;
	float xtarget, ytarget, ztarget;
	float lens_size;
	float servo1_angle;
	float servo2_angle;
	bool snap;
	
	void ClampAngle( float &a );
	bool AngleInRange( float a, float b, float range );
	
	
	};
	
	}
	
#endif


