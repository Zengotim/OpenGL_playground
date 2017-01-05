#version 150

in vec4 vPosition;		// Original position of the vertex
uniform mat4 mP;		// This is Perpsective matrix
uniform mat4 mV;		// This is camera matrix (View)
uniform mat4 mM;		// This is the Model matrix

out vec3 texCoords;

void main (void) {
	texCoords = normalize (vPosition.xyz);
	gl_Position = mP*mV*mM*vPosition;
}