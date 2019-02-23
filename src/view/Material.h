#ifndef LGC_MATERIAL_H
#define LGC_MATERIAL_H
// #include <GL/gl.h>

#include <string>



namespace LGC {

class Material {
			
			
	public: // ---------------------------------------------
			
	Material( std::string name = "default" ) : name(name), pushed(false) { 
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
		emission[0] = 0.0f;
		emission[1] = 0.0f;
		emission[2] = 0.0f;
		emission[3] = 0.0f;
		shine = 0.0f;
		}
		
	static Material CloneAndName( const Material &m, std::string name ) { 
		Material x = Material( name );
 		x.ambient[0] = m.ambient[0];
		x.ambient[1] = m.ambient[1];
		x.ambient[2] = m.ambient[2];
		x.ambient[3] = m.ambient[3];
		x.specular[0] = m.specular[0];
		x.specular[1] = m.specular[1];
		x.specular[2] = m.specular[2];
		x.specular[3] = m.specular[3];
		x.diffuse[0] = m.diffuse[0];
		x.diffuse[1] = m.diffuse[1];
		x.diffuse[2] = m.diffuse[2];
		x.diffuse[3] = m.diffuse[3];
		x.emission[0] = m.emission[0];
		x.emission[1] = m.emission[1];
		x.emission[2] = m.emission[2];
		x.emission[3] = m.emission[3];
		x.shine = m.shine;
		return x;
		}
		
	void SetAmbient( float r, float g, float b, float a );
	void SetSpecular( float r, float g, float b, float a );
	void SetDiffuse( float r, float g, float b, float a );
	void SetEmission( float r, float g, float b, float a );
	void SetShine( float x ) { shine = x; }
		
	inline std::string Name()			{ return name; }
	inline float* Ambient() 				{ return ambient; }
	inline float* Specular() 			{ return specular; }
	inline float* Diffuse() 				{ return diffuse; }
	inline float* Emission() 			{ return emission; }
	inline float  Shine() 				{ return shine; }
	
	void Switch( bool push = false ); 				
	void Pop();
	
	
	private: // --------------------------------------------
			
	float ambient[4];
	float specular[4];
	float diffuse[4];
	float emission[4];
	float shine;
	
	std::string name;
	bool pushed;
	
	static std::string active_material;
	
	};
	
}
	
#endif


