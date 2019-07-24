#ifndef LUZ_H
#define LUZ_H

#define AGA_DIRECTIONAL 1
#define AGA_POSITIONAL 2
#define AGA_SPOT 3

class Luz{

	public:
		// The default values that are listed here
		// are those defined by OpenGL
		// Our Default Light has different values
		// (see SetDefaultLight() )
		int type;
		int id; // GL_LIGHTx ; -1 not binded
		int switched; // TRUE => ON
		int needsUpdate; // TRUE / FALSE
		int white; // TRUE / FALSE
		int attenuation; // TRUE / FALSE
		float ambient[4]; // GL_AMBIENT : default (0.0, 0.0, 0.0, 1.0)
		float diffuse[4]; // GL_DIFFUSE : default (0.0, 0.0, 0.0, 1.0)
		// except for light zero (1.0, 1.0, 1.0, 1.0)
		float specular[4]; // GL_SPECULAR : default (0.0, 0.0, 0.0, 1.0)
		// except for light zero (1.0, 1.0, 1.0, 1.0)
		float position[4]; // GL_POSITION : default (0,0,1,0);
		// directional, in the direction of the -z
		float pointAtInfinity[4]; // these values do not refer to the components of
		// one vector they refer to :
		// the coordinates of one point placed in the infinite
		// ( as the point is in the infinite,
		// its 4th homogeneous coordinate must be 0.0 )
		float spotDirection[4]; // GL_SPOT_DIRECTION : default direction is (0,0,-1)
		// significant only when GL_SPOT_CUTOFF is not 180
		float spotExponent; // GL_SPOT_EXPONENT [0,128], default 0
		// 0 => uniform light distribution
		// higher spot => more focused light source,
		float spotCutOff; // GL_SPOT_CUTOFF [0,90] & 180, default 180
		// 180 => uniform light distribution
		float a; // GL_QUADRATIC_ATTENUATION
		float b; // GL_LINEAR_ATTENUATION
		float c; // GL_CONSTANT_ATTENUATION
		// I = I / ( a*delta*delta + b*delta + c )
		// delta : distance from light position to point
		// default a=0 b=0 c=1, no atenuation

		Luz();
		~Luz();
		void SetLight();
		void SetDefaultLight();
		void SwitchLight( int status );
		void Rotar_Luces_Longitud( float inc );
		void Rotar_Luces_Latitud( float inc );
		void Acercar_Alejar_Luces( float step );
		void Rotar_Spot_Latitud( float inc );
		void Rotar_Spot_Longitud( float inc );

};

#endif