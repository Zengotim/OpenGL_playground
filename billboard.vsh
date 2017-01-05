#version 150

// Combined our old stuff with the shader from Angel book

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;			// NEW!! - coming in from our OpenGL program
out vec2 texCoord;			// NEW!! - going out to the fragment shader

uniform mat4 mM;			// (M)odel matrix of model
uniform mat4 mV;			// (V)iew matrix of camera
uniform mat4 mP;			// (P)erspective matrix of camera
uniform mat4 mR;			// (R)otation of model just for rotating normals

uniform vec4 lightPosition;
uniform vec4 cameraPosition; 

out vec3 fN;				// Normal of vertex			
out vec3 fE;				// Vertex in Eye (camera) space
out vec3 fL;				// Light position

void main () {


	// Note: all this stuff is in world coordinates
	fN = (mR*vNormal).xyz;
	fE = (cameraPosition-mM*vPosition).xyz;
	fL = (lightPosition).xyz;

	// If it's not a direction light (e.g. the sun), then L depends on the position of the vertex
	if (lightPosition.w != 0.0) {
		fL = lightPosition.xyz - (vPosition).xyz;
	}

	gl_Position = mP*mV*mM*vPosition;
	texCoord = vTexCoord;	// NEW!! Notice, all we do is pass this through to the fragment shader
}