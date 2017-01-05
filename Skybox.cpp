#include "Skybox.h"

Skybox::Skybox(Shader* shader) : Model(shader) {
	// Just call the parent constructor
}

Skybox::~Skybox() {
	// Just call the parent destructor
}

void Skybox::loadCubeMapImages() {
	unsigned char* tempPicData;
	int width, height, size;

	// Create the first texture buffer
	glEnable (GL_TEXTURE_CUBE_MAP);
	glGenTextures (1, &tex_buffer_ID);		//Note: we inherited this from Model
	glBindTexture (GL_TEXTURE_CUBE_MAP, tex_buffer_ID);
	
	loadBitmapFromFile("NegX-b.bmp", &width, &height, &size, &tempPicData);
	glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, tempPicData);
	delete (tempPicData);
	loadBitmapFromFile("NegY-b.bmp", &width, &height, &size, &tempPicData);
	glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, tempPicData);
	delete (tempPicData);
	loadBitmapFromFile("NegZ-b.bmp", &width, &height, &size, &tempPicData);
	glTexImage2D (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, tempPicData);
	delete (tempPicData);
	loadBitmapFromFile("PosX-b.bmp", &width, &height, &size, &tempPicData);
	glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, tempPicData);
	delete (tempPicData);
	loadBitmapFromFile("PosY-b.bmp", &width, &height, &size, &tempPicData);
	glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, tempPicData);
	delete (tempPicData);
	loadBitmapFromFile("PosZ-b.bmp", &width, &height, &size, &tempPicData);
	glTexImage2D (GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, tempPicData);
	delete (tempPicData);

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	cubeMapID = glGetUniformLocation(shader->programID, "cubeMap");
	printf ("location for cubeMap: %d\n", cubeMapID);
		
	// Bind the current buffer to texture unit 0
	glUniform1i(cubeMapID, 0);
}

