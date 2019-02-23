#ifndef LGC_LIGHT_H
#define LGC_LIGHT_H
#include <GL/gl.h>
#include <string>
#include <map>

namespace LGC {

class Light {
			
			
	public: // ---------------------------------------------
			
	Light( std::string name = "default", GLenum light = 0, bool positional = false ) : name(name), light_number(light), on(false) { 
		spot_radius = 180.0f; // no spot
		spot_exponant = 2.0f;
		constant_attenuation = 1.5f;
		linear_attenuation = 0.5f;
		quadratic_attenuation = 0.2f; 
 		ambient[0] = 0.2f;
		ambient[1] = 0.2f;
		ambient[2] = 0.2f;
		ambient[3] = 1.0f;
		specular[0] = 0.2f;
		specular[1] = 0.2f;
		specular[2] = 0.2f;
		specular[3] = 1.0f;
		diffuse[0] = 0.6f;
		diffuse[1] = 0.6f;
		diffuse[2] = 0.6f;
		diffuse[3] = 1.0f;
		pos[0] = 1.0f;
		pos[1] = 1.0f;
		pos[2] = 1.0f;
		pos[3] = (positional ? 1.0f : 0.0f );
		direction[0] = -1.0f;
		direction[1] = -1.0f;
		direction[2] = -1.0f;
		}
		
	static Light CloneAndRename( Light& x, std::string name ) { 
		Light L = Light( name );
		L.light_number = x.light_number;
		L.spot_radius = x.spot_radius;
		L.spot_exponant = x.spot_exponant;
		L.constant_attenuation = x.constant_attenuation;
		L.linear_attenuation = x.linear_attenuation;
		L.quadratic_attenuation = x.quadratic_attenuation; 
 		L.ambient[0] = x.ambient[0];
		L.ambient[1] = x.ambient[1];
		L.ambient[2] = x.ambient[2];
		L.ambient[3] = x.ambient[3];
		L.specular[0] = x.specular[0];
		L.specular[1] = x.specular[1];
		L.specular[2] = x.specular[2];
		L.specular[3] = x.specular[3];
		L.diffuse[0] = x.diffuse[0];
		L.diffuse[1] = x.diffuse[1];
		L.diffuse[2] = x.diffuse[2];
		L.diffuse[3] = x.diffuse[3];
		L.pos[0] = x.pos[0];
		L.pos[1] = x.pos[1];
		L.pos[2] = x.pos[2];
		L.pos[3] = x.pos[3];
		L.direction[0] = x.direction[0];
		L.direction[1] = x.direction[1];
		L.direction[2] = x.direction[2];
		return L;
		}
		
	void LookAt( float x, float y, float z );
	
	void SetAmbient( float r, float g, float b, float a );
	void SetSpecular( float r, float g, float b, float a );
	void SetDiffuse( float r, float g, float b, float a );
	void SetPosition( float x, float y, float z );
	void SetDirection( float x, float y, float z );
	void SetDirectional( bool x );
	void SetSpotRadius( float x );
	void SetSpotExponant( float x );
	void SetConstantAttenuation( float x );
	void SetLinearAttenuation( float x );
	void SetQuadraticAttenuation( float x );
	
	inline std::string Name()			{ return name; }
	inline float* Ambient() 				{ return ambient; }
	inline float* Specular() 			{ return specular; }
	inline float* Diffuse() 				{ return diffuse; }
	inline float  Xpos()				{ return pos[0]; }
	inline float  Ypos()				{ return pos[1]; }
	inline float  Zpos()				{ return pos[2]; }
	inline bool   Directional()			{ return pos[3]; }
	inline float  SpotRadius() 			{ return spot_radius; }
	inline float  SpotExponant() 			{ return spot_exponant; }
	inline float  ConstantAttenuation() 	{ return constant_attenuation; }
	inline float  LinearAttenuation() 		{ return linear_attenuation; }
	inline float  QuadraticAttenuation() 	{ return quadratic_attenuation; } 
	inline GLenum LightNumber()			{ return light_number; }
	
	void TurnOn();
	void TurnOff();
	
	void Draw();
	
	void AddToScene();
	
	void RetireMyLightNumber();
	void SnagANewLightNumber();
	
	static GLenum NewLightNumber();
	static void RetireLightNumber( GLenum n );
		
		
	private: // --------------------------------------------
	
	std::string name;		
	GLenum light_number;
	bool on;
	float ambient[4];
	float specular[4];
	float diffuse[4];
	float pos[4];
	float direction[3];
	float spot_radius;
	float spot_exponant;
	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;
	
	static std::map< GLenum, bool > lights_available;
	
	};
	
}
	
#endif


