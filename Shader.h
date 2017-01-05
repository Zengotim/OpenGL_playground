#ifndef SHADER
#define SHADER
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <stdio.h>

class Shader {
public:
	Shader(const char* vertexProgram, const char* fragmentProgram);
	~Shader();
	GLint programID;
private:
	char* readFile(const char* filename);
	bool compiledStatus (GLint shaderID);
	bool linkedStatus (GLint programID);
	GLint makeVertexShader(char* shaderFile);
	GLint makeFragmentShader(char* shaderFile);
	GLint makeShaderProgram(GLint vertexShaderID, GLint fragmentShaderID);
};
#endif