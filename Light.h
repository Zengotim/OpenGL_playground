#ifndef LIGHT
#define LIGHT
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Shader.h"

class Light {
public:
	Light(GLfloat* position, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular);
	void setLightPositionInShader(GLuint shaderID);
	void setLightPosition(GLfloat* newPosition);

	GLfloat*	position;
	GLfloat*	ambient;
	GLfloat*	diffuse;
	GLfloat*	specular;

private:
	GLuint		lightPositionID;		// The ID of the lightPosition variable in the current shader
};
#endif