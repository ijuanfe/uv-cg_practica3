#include <GL/glut.h>
#include "Luz.h"
#include "Vector_tools.h"

Luz::Luz(){
	SetDefaultLight( );
}

Luz::~Luz(){};

void Luz::SetDefaultLight( ) {
	int ierr = 0;
	float intensity;
	float vx, vy, vz;
	// directional light
	type = AGA_DIRECTIONAL;
	id = -1;
	switched = FALSE;
	white = TRUE;
	attenuation = TRUE; // Poner en true y calculñar la atenuación
	needsUpdate = TRUE;
	intensity = 0.0f;
	ambient[0] = intensity; //R
	ambient[1] = intensity; //G
	ambient[2] = intensity; //B
	ambient[3] = 1.0f; 
	intensity = 0.8f; // 1 / r^2 Modificar
	diffuse[0] = intensity;
	diffuse[1] = intensity;
	diffuse[2] = intensity;
	diffuse[3] = 1.0f;
	intensity = 0.0f;
	specular[0] = intensity;
	specular[1] = intensity;
	specular[2] = intensity;
	specular[3] = 1.0f;
	position[0] = 1.0f;
	position[1] = 1.0f;
	position[2] = 1.0f;
	position[3] = 1.0f;
	vx = 1.0f; vy = 1.0f; vz = 1.0f;
	VectorNormalize( &ierr, &vx, &vy, &vz );
	pointAtInfinity[0] = vx;
	pointAtInfinity[1] = vy;
	pointAtInfinity[2] = vz; // The light is "placed" at point "V" in the infinite
	pointAtInfinity[3] = 0.0f; // So light rays flow in the direction of vector "-v"
	vx = -1.0f; vy = -1.0f; vz = -1.0f;
	VectorNormalize( &ierr, &vx, &vy, &vz );
	spotDirection[0] = vx;
	spotDirection[1] = vy;
	spotDirection[2] = vz;
	spotDirection[3] = 0.0f;
	spotExponent = 10.0f;
	spotCutOff = 30.0f; // must be degrees
	a = 0.1f; // GL_QUADRATIC_ATTENUATION
	b = 0.0f; // GL_LINEAR_ATTENUATION
	c = 1.0f; // GL_CONSTANT_ATTENUATION
}

void Luz::SwitchLight( int status ){

	if( id < GL_LIGHT0 ) return;

	switched = status;
	if( status ) {
		glEnable( id );
		needsUpdate = TRUE;
	}
	else {
		glDisable( id );
	}

}

void Luz::SetLight( ) {

	int lightId;
	if( ! switched ) return;
	if( id < GL_LIGHT0 ) return;
	
	lightId = id;
	// Geometric parameters will be always set when the scene is redrawn
	if( type == AGA_DIRECTIONAL ) {
		glLightfv( lightId, GL_POSITION, pointAtInfinity );
	}
	else if( type == AGA_POSITIONAL ) {
		glLightfv( lightId, GL_POSITION, position );
	}
	else {
		glLightfv( lightId, GL_POSITION, position );
		glLightfv( lightId, GL_SPOT_DIRECTION, spotDirection );
	}

	// These other parameters are seldom changed
	// So, they will be set only when any one of them is changed. The user interface
	// must set "needsUpdate" to TRUE, whenever any of these parameters changes
	if( needsUpdate ) {
		needsUpdate = FALSE;
		glLightfv( lightId, GL_AMBIENT, ambient );
		glLightfv( lightId, GL_DIFFUSE, diffuse );
		glLightfv( lightId, GL_SPECULAR, specular );
		if( type == AGA_SPOT ) {
			glLightf( lightId, GL_SPOT_EXPONENT, spotExponent );
			glLightf( lightId, GL_SPOT_CUTOFF, spotCutOff );
		}
		else {
			glLighti( lightId, GL_SPOT_EXPONENT, 0 );
			glLighti( lightId, GL_SPOT_CUTOFF, 180 );
		}
		if( ! attenuation || type == AGA_DIRECTIONAL ) {
			glLighti( lightId, GL_CONSTANT_ATTENUATION, 1 );
			glLighti( lightId, GL_LINEAR_ATTENUATION, 0 );
			glLighti( lightId, GL_QUADRATIC_ATTENUATION, 0 );
		}
		else {
			glLightf( lightId, GL_CONSTANT_ATTENUATION, c );
			glLightf( lightId, GL_LINEAR_ATTENUATION, b );
			glLightf( lightId, GL_QUADRATIC_ATTENUATION, a );
		}
	}
}

void Luz::Rotar_Luces_Longitud( float inc ) {
	float vIn[3];
	vIn[0]= position[0] ;
	vIn[1]= position[1] ;
	vIn[2]= position[2] ;
	VectorRotY( vIn, inc );
	position[0] = vIn[0];
	position[2] = vIn[2];
}

void Luz::Rotar_Luces_Latitud( float inc ) {
	float vIn[3];
	vIn[0]= position[0] ;
	vIn[1]= position[1] ;
	vIn[2]= position[2] ;
	VectorRotXZ( vIn, inc, TRUE );
	position[0] = vIn[0];
	position[1] = vIn[1];
	position[2] = vIn[2];
}

void Luz::Acercar_Alejar_Luces( float step ) {
	int ierr;
	float vaX, vaY, vaZ;
	float modulo;
	vaX= position[0];
	vaY= position[1];
	vaZ= position[2];
	VectorNormalize( &ierr, &vaX, &vaY, &vaZ );
	vaX= step * vaX;
	vaY= step * vaY;
	vaZ= step * vaZ;
	// Set new position
	modulo = sqrt(pow(position[0] + vaX,2) + pow(position[1] + vaY,2) + pow(position[2] + vaZ,2));

	if(modulo < 0.8f) return;

	diffuse[0] = ( 1.0f / ( modulo*modulo ) );
	diffuse[1] = ( 1.0f / ( modulo*modulo ) );
	diffuse[2] = ( 1.0f / ( modulo*modulo ) );
	
	position[0] += vaX;
	position[1] += vaY;
	position[2] += vaZ;

	needsUpdate = TRUE;

}

void Luz::Rotar_Spot_Latitud( float inc ) {
	float vIn[3];
	vIn[0]= spotDirection[0] ;
	vIn[1]= spotDirection[1] ;
	vIn[2]= spotDirection[2] ;
	VectorRotXZ( vIn, inc, TRUE );
	spotDirection[0] = vIn[0];
	spotDirection[1] = vIn[1];
	spotDirection[2] = vIn[2];
}
void Luz::Rotar_Spot_Longitud( float inc ) {
	float vIn[3];
	vIn[0]= spotDirection[0] ;
	vIn[1]= spotDirection[1] ;
	vIn[2]= spotDirection[2] ;
	VectorRotY( vIn, inc );
	spotDirection[0] = vIn[0];
	spotDirection[2] = vIn[2];
}

