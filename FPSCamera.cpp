#include "FPSCamera.h"


FPSCamera::FPSCamera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, bool isFlying = true)
{
	// The View matrix (the position and orientation of the camera)
	this->mV = new GLfloat[16];
	MathHelper::makeIdentity(mV);

	// The Perspective matrix - note that fov, aspect and the planes reside in the P matrix.
	this->mP = new GLfloat[16];
	this->setPerspective(fov, aspect, nearPlane, farPlane);

	// Some temporary matrices for multiplying
	this->tempMatrix = new GLfloat[16];
	this->tempMatrix2 = new GLfloat[16];
	this->tempMatrix3 = new GLfloat[16];

	this->camPos = new GLfloat[4];

	MathHelper::makeIdentity(tempMatrix);
	MathHelper::makeIdentity(tempMatrix2);
	MathHelper::makeIdentity(tempMatrix3);

	this->isFlying = isFlying;
	counter = 0;
	rotX = rotY = rotZ = posX = posY = posZ = 0;
	
}
FPSCamera::~FPSCamera()
{
	delete(mP);
	delete(mV);
	delete(tempMatrix);
	delete(tempMatrix2);
	delete(tempMatrix3);
}

#pragma region TRANSFORM_METHODS
void FPSCamera::rotateX(GLfloat amount)
{
	this->rotX += amount;
	if (isFlying) {
		MathHelper::copyMatrix(this->mV, tempMatrix);
		MathHelper::makeRotateX(tempMatrix2, amount);
		MathHelper::matrixMult4x4(this->mV, tempMatrix2, tempMatrix);
	}
	else {
		if (rotX >= 3.14159/2) rotX = 3.14159/2;
		else if (rotX <= -3.14159/2) rotX = -3.14159/2;
	}
}
void FPSCamera::rotateY(GLfloat amount)
{
	this->rotY += amount;
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeRotateY(tempMatrix2, amount);
	MathHelper::matrixMult4x4(this->mV, tempMatrix2, tempMatrix);
	
}
void FPSCamera::rotateZ(GLfloat amount)
{
	this->rotZ += amount;
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeRotateZ(tempMatrix2, amount);
	MathHelper::matrixMult4x4(this->mV, tempMatrix2, tempMatrix);
	
}

// Strafing
void FPSCamera::moveX(GLfloat amount)
{
	this->posX -= cos(rotY)*amount;
	this->posZ += sin(rotY)*amount;
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeTranslate(tempMatrix2, amount, 0.0f, 0.0f);
	MathHelper::matrixMult4x4(this->mV, tempMatrix2, tempMatrix);
}
void FPSCamera::moveY(GLfloat amount)
{
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeTranslate(tempMatrix2, 0.0f, amount, 0.0f);
	MathHelper::matrixMult4x4(this->mV, tempMatrix2, tempMatrix);	
}
void FPSCamera::moveZ(GLfloat amount)
{
	this->posX -= sin(rotY)*amount;
	this->posZ -= cos(rotY)*amount;
	MathHelper::copyMatrix(this->mV, tempMatrix);
	MathHelper::makeTranslate(tempMatrix2, 0.0f, 0.0f, amount);
	MathHelper::matrixMult4x4(this->mV, tempMatrix2, tempMatrix);
	
}
#pragma endregion TRANSFORM_METHODS

void FPSCamera::setPerspective(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane)
{
	GLfloat f = 1.0f/tan(fov*3.1415926f/360.0f);
	for (int i = 0; i < 16; i++) {
		mP[i] = 0.0f;
	}
	mP[0] = f/aspect;
	mP[5] = f;
	mP[10] = ((farPlane+nearPlane)/(nearPlane-farPlane));
	mP[11] = -1;
	mP[14] = (2.0f*farPlane*nearPlane)/(nearPlane-farPlane);
	mP[15] = 0;
}

// For skybox only
void FPSCamera::setNonMovingVPInShader(GLuint shaderID)
{
	glUseProgram(shaderID);

	// Find the location of the "mP" variable in the GPU vertex shader
	GLuint mpLoc = glGetUniformLocation(shaderID, "mP");
	// Send our variable mP to the GPU shader's mP variable
	glUniformMatrix4fv(mpLoc, 1, GL_TRUE, mP);

	

	// Do the same thing: set the GPU's mV in the shader passing our local mV member variable
	GLuint mvLoc = glGetUniformLocation(shaderID, "mV");
		
	MathHelper::copyMatrix(this->mV, tempMatrix3);
	
	// MAGIC HAPPENS HERE
	tempMatrix3[3] = 0;
	tempMatrix3[7] = 0;
	tempMatrix3[11] = 0;

	if (!isFlying) {
		// Make a rotation matrix only using the X and Y rotations (not Z)
		MathHelper::makeRotateX(tempMatrix2, rotX);
		MathHelper::matrixMult4x4(tempMatrix, tempMatrix2, tempMatrix3);
		glUniformMatrix4fv(mvLoc, 1, GL_TRUE, tempMatrix);
	}
	else {
		glUniformMatrix4fv(mvLoc, 1, GL_TRUE, tempMatrix3);
	}
}

void FPSCamera::setVPInShader(GLuint shaderID)
{
	glUseProgram(shaderID);

	// Find the location of the "mP" variable in the GPU vertex shader
	GLuint mpLoc = glGetUniformLocation(shaderID, "mP");
	// Send our variable mP to the GPU shader's mP variable
	glUniformMatrix4fv(mpLoc, 1, GL_TRUE, mP);

	// Do the same thing: set the GPU's mV in the shader passing our local mV member variable
	GLuint mvLoc = glGetUniformLocation(shaderID, "mV");
	
	camPos[0] = posX;
	camPos[1] = posY;
	camPos[2] = posZ;
	GLuint camPosLoc = glGetUniformLocation(shaderID, "cameraPosition");
	if (camPosLoc != -1) {
		glUniform4fv(camPosLoc, 1, camPos);
	}

	if (!isFlying) {
		// Make a rotation matrix only using the X and Y rotations (not Z)
		MathHelper::makeRotateX(tempMatrix2, rotX);
		MathHelper::matrixMult4x4(tempMatrix, tempMatrix2, this->mV);
		glUniformMatrix4fv(mvLoc, 1, GL_TRUE, tempMatrix);
	}
	else {
		glUniformMatrix4fv(mvLoc, 1, GL_TRUE, mV);
	}
}