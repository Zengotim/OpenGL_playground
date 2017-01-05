#include "Shader.h"

Shader::Shader(const char* vertexFilename, const char* fragmentFilename){
	// Read in the vertex program file
	char* vertexCode = readFile(vertexFilename);
	// Read in the fragment program file
	char* fragmentCode = readFile(fragmentFilename);
	// Check to make sure neither one is null.  If so, print an error and exit
	if ((vertexCode==NULL)||(fragmentCode==NULL)) {
		printf ("Can't find shader file\n");
		exit(-1);
	}
	// Make a vertex shader
	GLint vertexID = makeVertexShader(vertexCode);
	// Make a fragment shader
	GLint fragmentID = makeFragmentShader(fragmentCode);
	// Make the shader program and assign it to the "programID" variable
	programID = makeShaderProgram(vertexID, fragmentID);
	printf ("Shader ID is %d\n", programID);
	// Clean up memory (the first two variables you created)
	delete (vertexCode);
	delete (fragmentCode);
}

char* Shader::readFile (const char* filename) {
	// Open the file
	FILE* fp = fopen(filename, "r");

	// Go to the beginning and get the length of the file
	fseek (fp, 0, SEEK_END);
	long file_length = ftell(fp);

	// Go back to the beginning and read the contents
	fseek (fp, 0, SEEK_SET);
	char* contents = new char[file_length+1];
	for (int i = 0; i < file_length+1; i++) {
		contents[i] = 0;
	}
	fread (contents, 1, file_length, fp);
	contents[file_length] = '\0';
	fclose(fp);

	return contents;
}

#pragma region VERTEX AND FRAGMENT SHADER FUNCTIONS
/// This function determines if a shader (either vertex or fragment) compiled correctly
bool Shader::compiledStatus(GLint shaderID){
	GLint compiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled) {
		return true;
	}
	else {
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* msgBuffer = new char[logLength];
		glGetShaderInfoLog(shaderID, logLength, NULL, msgBuffer);
		printf ("%s\n", msgBuffer);
		delete (msgBuffer);
		return false;
	}
}
// This function takes in the source code for a *vertex* shader,
// compiles it, and returns the ID (number) of the shader. It returns
// -1 if the shader didn't compile
GLint Shader::makeVertexShader (char* shaderSource) {
	GLint vShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource (vShaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(vShaderID);
	bool compiledCorrectly = compiledStatus(vShaderID);
	if (compiledCorrectly) {
		return vShaderID;
	}
	return -1;
}
// This function takes in the source code for a *fragment* shader,
// compiles it, and returns the ID (number) of the shader. It returns
// -1 if the shader didn't compile
GLint Shader::makeFragmentShader (char* shaderSource) {
	GLint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource (fShaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(fShaderID);
	bool compiledCorrectly = compiledStatus(fShaderID);
	if (compiledCorrectly) {
		return fShaderID;
	}
	return -1;
}
#pragma endregion VERTEX AND FRAGMENT SHADER FUNCTIONS

#pragma region SHADER PROGRAM FUNCTIONS
// A helper function that takes in a program ID and returns whether
// or not it linked correctly.  If not, it prints out an error
// message letting you know what the problem was. This will
// be called from makeShaderProgram()
bool Shader::linkedStatus(GLint programID){
	GLint linked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &linked);
	if (linked) {
		return true;
	}
	else {
		GLint logLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		char* msgBuffer = new char[logLength];
		glGetProgramInfoLog(programID, logLength, NULL, msgBuffer);
		printf ("%s\n", msgBuffer);
		delete (msgBuffer);
		return false;
	}
}

// A function that takes in the IDs for vertex and fragment shader,
// and returns the ID of a shader program that has been linked.
GLint Shader::makeShaderProgram (GLint vertexShaderID, GLint fragmentShaderID) {
	printf ("Linking %d with %d\n", vertexShaderID, fragmentShaderID);
	GLint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram (programID);

	bool linked = linkedStatus (programID);

	if (linked) {
		return programID;
	}
	return -1;
}

Shader::~Shader(){
	glDeleteProgram(programID);
}
#pragma endregion SHADER PROGRAM FUNCTIONS