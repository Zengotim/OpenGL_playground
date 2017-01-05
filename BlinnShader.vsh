#version 150

// Combined our old stuff with the shader from Angel book

in vec4 vPosition;
in vec4 vNormal;
uniform mat4 mM;
uniform mat4 mV;
uniform mat4 mP;
uniform mat4 mR;				// for the rotation matrix only

uniform vec4 lightPosition;		// NEW!
uniform vec4 cameraPosition; 

out vec3 fN;					// NEW!  These go to the fragment shader for interpolation
out vec3 fE;
out vec3 fL;

void main () {

    // Note: all this stuff is in world coordinates
	fN = (mR*vNormal).xyz;
	fE = (cameraPosition-mM*vPosition).xyz;
	fL = (lightPosition).xyz;
	
	// If it's not a direction light (e.g. the sun), then L depends on the position of the vertex
	if (lightPosition.w != 0.0) {
		fL = lightPosition.xyz - (vPosition).xyz;
	}

	// This is in camera coordinates
	gl_Position = mP*mV*mM*vPosition;
}