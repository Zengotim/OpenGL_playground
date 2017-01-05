#version 150

out vec4 vFragColor;
uniform samplerCube cubeMap;
in vec3 texCoords;

void main (void) {
	vFragColor = texture (cubeMap, texCoords);
}
