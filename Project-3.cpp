// Project-3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define  _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include <vector>
#include "Shader.h"
#include "Model.h"
#include "FPSCamera.h"
#include "MathHelper.h"
#include "Light.h"
#include "WorldActor.h"
#include "Projectile.h"
#include "Skybox.h"

std::vector<WorldActor*> *actors = new std::vector<WorldActor*>();
std::vector<WorldActor*> *rails = new std::vector<WorldActor*>();
std::vector<WorldActor*> *crowd = new std::vector<WorldActor*>();
Model* model;
Model* planeModel;
Model* trackModel;
Model* dragonModel;
Model* teapotModel;
Model* railModel;
Model* trainModel;
FPSCamera* camera;
Light*	light;	
Skybox* skybox;
GLfloat* rotXMatrix;
GLfloat* rotYMatrix;
GLfloat* rotZMatrix;
GLfloat* rotMatrix;
GLfloat* transMatrix;

GLfloat* scaleMatrix;
GLfloat* tempMatrix;
GLfloat* tempMatrix2;
GLfloat zVel, xVel, maxSpeed;
GLfloat fov;
GLboolean zooming;
GLfloat theta=0;

void zoomIn();
void zoomOut();
GLfloat oldMouseX;
GLfloat oldMouseY;
bool firstTime = true;
int framecount = 0;

void render() {
	framecount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (zooming) zoomIn();
	else if (fov <= 90) zoomOut();
	camera->moveZ (zVel);
	camera->moveX (xVel);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//*******************************************************
	//
	//              Process and render
	//
	//*******************************************************

	// Render loop for dynamic actors
	for (unsigned i = 0; i < actors->size(); ++i){
		//increment the angle value for figure-8 path
		if (framecount % 2 == 0){
			actors->at(i)->setTheta(actors->at(i)->getTheta()+(0.02f));
			if (actors->at(i)->getTheta() >= 2*M_PI){
				actors->at(i)->setTheta(0.0f);
				actors->at(i)->toggleDirection();
			}
			// fly in a figure 8
			actors->at(i)->setVelX(sin(actors->at(i)->getTheta())/3*actors->at(i)->getDirection());
			actors->at(i)->setVelZ(cos(actors->at(i)->getTheta())/3);
			// face the direction you are flying
			actors->at(i)->setYangle(actors->at(i)->getTheta()*actors->at(i)->getDirection());
			// Add position + velocity
			actors->at(i)->setX(actors->at(i)->getX()+actors->at(i)->getVelX());
			actors->at(i)->setY(actors->at(i)->getY()+actors->at(i)->getVelY());
			actors->at(i)->setZ(actors->at(i)->getZ()+actors->at(i)->getVelZ());
		}
		//Great cheat here to find the positions for models!
		if (actors->at(i)->getTheta()>6){
			printf("  Theta:  ");
			printf("%f", actors->at(i)->getTheta());
			printf("  posX:  ");
			printf("%f", actors->at(i)->getX());
			printf("  posZ:  ");
			printf("%f", actors->at(i)->getZ());
		}
		// make scale
		MathHelper::makeScale(scaleMatrix, actors->at(i)->getScale(), 
			actors->at(i)->getScale(), actors->at(i)->getScale());
		// Make rotate
		MathHelper::makeRotateX(rotXMatrix, actors->at(i)->getXangle());
		MathHelper::makeRotateY(rotYMatrix,-actors->at(i)->getYangle());
		MathHelper::makeRotateZ(rotZMatrix, actors->at(i)->getZangle());
		// Make translate
		MathHelper::makeTranslate(transMatrix, actors->at(i)->getX(),  
			actors->at(i)->getY(),  actors->at(i)->getZ());
		//Combine for final tranformation
		MathHelper::matrixMult4x4(tempMatrix, rotXMatrix, rotYMatrix);
		MathHelper::matrixMult4x4(rotMatrix, tempMatrix, rotZMatrix);
		MathHelper::matrixMult4x4(tempMatrix2, transMatrix, rotMatrix);
		MathHelper::matrixMult4x4(actors->at(i)->myModel->mM, tempMatrix2, scaleMatrix);
		// Set camera, lighting, then render
		camera->setVPInShader(actors->at(i)->myModel->shader->programID);
		light->setLightPositionInShader(actors->at(i)->myModel->shader->programID);
		actors->at(i)->myModel->render();
	}

	// Render loop for guardrails
	for (unsigned i = 0; i < rails->size(); ++i){
		// make scale
		MathHelper::makeScale(scaleMatrix, rails->at(i)->getScale(), 
			rails->at(i)->getScale(), rails->at(i)->getScale());
		// Make rotate
		MathHelper::makeRotateX(rotXMatrix, rails->at(i)->getXangle());
		MathHelper::makeRotateY(rotYMatrix,-rails->at(i)->getYangle());
		MathHelper::makeRotateZ(rotZMatrix, rails->at(i)->getZangle());
		// Make translate
		MathHelper::makeTranslate(transMatrix, rails->at(i)->getX(),  
			rails->at(i)->getY(),  rails->at(i)->getZ());
		//Combine for final tranformation
		MathHelper::matrixMult4x4(tempMatrix, rotXMatrix, rotYMatrix);
		MathHelper::matrixMult4x4(rotMatrix, tempMatrix, rotZMatrix);
		MathHelper::matrixMult4x4(tempMatrix2, transMatrix, rotMatrix);
		MathHelper::matrixMult4x4(rails->at(i)->myModel->mM, tempMatrix2, scaleMatrix);
		// Set camera, lighting, then render
		camera->setVPInShader(rails->at(i)->myModel->shader->programID);
		light->setLightPositionInShader(rails->at(i)->myModel->shader->programID);
		rails->at(i)->myModel->render();
	}

		// spectators tried (and failed) to use alpha for transparency
		//glEnable(GL_BLEND);
		//glBlendEquation(GL_MAX);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glAlphaFunc(GL_GREATER, 0.5);
		//glEnable(GL_ALPHA_TEST);

		for (unsigned i = 0; i < crowd->size(); ++i){
			crowd->at(i)->setTheta(crowd->at(i)->getTheta()+0.5f);
			if (crowd->at(i)->getTheta() > 2*M_PI) crowd->at(i)->setTheta(0.0f);
			// make scale
			MathHelper::makeScale(scaleMatrix, crowd->at(i)->getScale(), 
				crowd->at(i)->getScale(), crowd->at(i)->getScale());
			
			// Make rotate
			MathHelper::makeRotateX(rotXMatrix, crowd->at(i)->getXangle());
			MathHelper::makeRotateY(rotYMatrix, crowd->at(i)->getYangle());
			MathHelper::makeRotateZ(rotZMatrix, crowd->at(i)->getZangle());
			// Make translate
			MathHelper::makeTranslate(transMatrix, crowd->at(i)->getX(),  
				crowd->at(i)->getY() + sin(crowd->at(i)->getTheta())/4,  crowd->at(i)->getZ());
			//Combine for final tranformation
			MathHelper::matrixMult4x4(tempMatrix, rotXMatrix, rotYMatrix);
			MathHelper::matrixMult4x4(rotMatrix, tempMatrix, rotZMatrix);
			MathHelper::matrixMult4x4(tempMatrix2, transMatrix, rotMatrix);
			MathHelper::matrixMult4x4(crowd->at(i)->myModel->mM, tempMatrix2, scaleMatrix);
			 //Set camera, lighting, then render
			camera->setVPInShader(crowd->at(i)->myModel->shader->programID);
			light->setLightPositionInShader(crowd->at(i)->myModel->shader->programID);
			crowd->at(i)->myModel->render();
		}

	// The racetrack
	MathHelper::makeScale(scaleMatrix, 1.87f, 1.0f, 1.87f);
	MathHelper::makeRotateY(rotMatrix, M_PI/2);
	MathHelper::makeTranslate(transMatrix, 19.0f, -1.8f, -10.0f);
	MathHelper::matrixMult4x4(tempMatrix, transMatrix, rotMatrix);
	MathHelper::matrixMult4x4(trackModel->mM, tempMatrix, scaleMatrix);
	camera->setVPInShader(trackModel->shader->programID);
	light->setLightPositionInShader(trackModel->shader->programID);
	trackModel->render();

	theta += 0.1f;

	//Dragon
	MathHelper::makeScale(scaleMatrix, 5.0f, 5.0f, 5.0f);
	MathHelper::makeRotateY(rotMatrix, theta);
	MathHelper::makeTranslate(transMatrix, -15.5f, 1.0f, -10.0f);
	MathHelper::matrixMult4x4(tempMatrix, transMatrix, rotMatrix);
	MathHelper::matrixMult4x4(dragonModel->mM, tempMatrix, scaleMatrix);
	camera->setVPInShader(dragonModel->shader->programID);
	light->setLightPositionInShader(dragonModel->shader->programID);
	dragonModel->render();

	//teapot
	MathHelper::makeScale(scaleMatrix, 5.0f, 5.0f, 5.0f);
	MathHelper::makeRotateY(rotMatrix, -theta);
	MathHelper::makeTranslate(transMatrix, 18.7f, 0.0f, -10.0f);
	MathHelper::matrixMult4x4(tempMatrix, transMatrix, rotMatrix);
	MathHelper::matrixMult4x4(teapotModel->mM, tempMatrix, scaleMatrix);
	camera->setVPInShader(teapotModel->shader->programID);
	light->setLightPositionInShader(teapotModel->shader->programID);
	teapotModel->render();
	
	//The skybox
	glFrontFace(GL_CW);
	MathHelper::makeScale(skybox->mM, 1000.0f, 1000.0f, 1000.0f);
	camera->setNonMovingVPInShader(skybox->shader->programID);
	skybox->render();
	glFrontFace(GL_CCW);

	//the train model, moves with camera
	MathHelper::makeScale(scaleMatrix, 0.3f, 0.3f, 0.3f);
	MathHelper::makeTranslate(transMatrix, 0.0f, -1.3f, 0.0f);
	MathHelper::makeRotateY(rotYMatrix, -camera->rotY+M_PI);
	MathHelper::matrixMult4x4(tempMatrix, transMatrix, rotYMatrix);
	MathHelper::matrixMult4x4(trainModel->mM, tempMatrix, scaleMatrix);
	camera->setNonMovingVPInShader(trainModel->shader->programID);
	trainModel->render();

	// The plane! The plane!
	MathHelper::makeScale(tempMatrix, 100.0f, 100.0f, 100.0f);
	MathHelper::makeTranslate(tempMatrix2, 0.0f, -1.9f, -10.0f);
	MathHelper::matrixMult4x4(planeModel->mM, tempMatrix2, tempMatrix);
	camera->setVPInShader(planeModel->shader->programID);
	planeModel->render();

	glutSwapBuffers();
	glutPostRedisplay();
}

// Any time the window is resized, this function gets called.  It's setup to the 
// "glutReshapeFunc" in main.
void changeViewport(int w, int h){
	glViewport(0, 0, w, h);
}

//zooming, duh
void zoomIn() {
	// DO IT! <- what are you, Nike?
	fov -= 10.0f;
	if (fov < 5.0f)
		fov = 5.0f;
	camera->setPerspective(fov, 1.0f, 1.0f, 1000.0f);
}

void zoomOut() {
	fov += 10;
	if (fov > 60)
		fov = 60;
	camera->setPerspective(fov, 1.0f, 1.0f, 1000.0f);
}

#pragma region GLUT CALLBACK FUNCTIONS N STUFF
// 2) For the keyboard functions, you need to change the x and z velocities based on
//    the input. In keyboardFunc, you should map WASD keys to those velocities (set them to
//    maxSpeed or -maxSpeed. If the keyboardUpFunction is called, you should set those velocities
//    to 0
void keyboardFunc (unsigned char key, int x, int y) {

	switch (key) {
		case 033:
		case 'q':
			exit(1);
			break;
		case 'w': zVel = maxSpeed; break;
		case 's': zVel = -maxSpeed; break;
		case 'a': xVel = maxSpeed; break;
		case 'd': xVel = -maxSpeed; break;

	}

}

void keyboardUpFunc (unsigned char key, int x, int y) {
	zVel = 0;
	xVel = 0;
}

// The mouse button callback.  If the user right-clicks and holds, zoom in.
// Basically, you're just setting "zooming" to true or false. See the render method for more info.
void mouseReader(int button, int state, int x, int y) {
	printf ("%d, %d\n", button, state);
	if (button == 2) {
		if (state == 0) {
			zooming = true;
		}
		else {
			zooming = false;
		}
	}
	if (button == 0) {
		if (state == 0) {

		}
	}
	
}

// Compare the current mouse position with the old one to figure out which way to rotate the camera.
// We call these "deltas" or "differences" in x and y
void look (int x, int y) {
	if (firstTime) {
		firstTime = false;
		oldMouseX = x;
		oldMouseY = y;
		return;
	}
	GLfloat deltaX = oldMouseX - x;
	GLfloat deltaY = oldMouseY - y;
	if (zooming){
		deltaX /= 10;
		deltaY /= 10;
	}
	if (camera->isFlying){
		camera->rotateX(deltaY/50);
		camera->rotateZ(deltaX/50);
	}
	else{
		camera->rotateX(deltaY/50);
		camera->rotateY(deltaX/50);
	}
	
	// Update oldMouseX and oldMouseY to be the current x and y
	oldMouseX = x;
	oldMouseY = y;
}
#pragma endregion GLUT CALLBACK FUNCTIONS N STUFF


int _tmain(int argc, char** argv)
{
	float t;
	// Initialize GLUT
	glutInit(&argc, argv);

	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(800, 800);
	// Create the window with the title "Triangle"
	glutCreateWindow("Timothy Goodson OpenGL Project");
	// Bind the two functions (above) to respond when necessary
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutMouseFunc (mouseReader);
	glutKeyboardFunc(keyboardFunc);
	glutKeyboardUpFunc(keyboardUpFunc);
	glutPassiveMotionFunc(look);
	glutMotionFunc(look);
	
	// Remember, we always init GLEW after we create a window...
	// This asks the GPU driver for all of those gl functions
	if (GLEW_OK != glewInit()) {
		exit(1);
	}
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable (GL_DEPTH_TEST);
	// i want backface culling on in my labs thank you very much ;-)
	glEnable(GL_CULL_FACE);

	
	//**************************************************************
	// Shaders******************************************************
	const char* vertexProgram = "../Project-3/SkyboxShader.vsh";
	const char* fragmentProgram = "../Project-3/SkyboxShader.fsh";
	Shader* skyboxShader = new Shader(vertexProgram, fragmentProgram);

	vertexProgram = "../Project-3/TextureShader.vsh";
	fragmentProgram = "../Project-3/TextureShader.fsh";
	Shader* textureShader = new Shader(vertexProgram, fragmentProgram);

	vertexProgram = "../Project-3/trackShader.vsh";
	fragmentProgram = "../Project-3/trackShader.fsh";
	Shader* trackShader = new Shader(vertexProgram, fragmentProgram);

	vertexProgram = "../Project-3/billboard.vsh";
	fragmentProgram = "../Project-3/billboard.fsh";
	Shader* billboardShader = new Shader(vertexProgram, fragmentProgram);



	
	//*************************************************************
	// Materials***************************************************
	GLfloat grassmatAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat grassmatDiffuse[] = {0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat grassmatSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat grassshininess = 30.0f;

	GLfloat matAmbient[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat matDiffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat shininess = 100.0f;

	
	//*************************************************************
	//
	//                  make all the assets
	//
	//*************************************************************

	// This is a directional light! (not a pokemon)
	printf("=========== LIGHT ==================\n");
	GLfloat lightPos[] = {-45.0f, 100.0f, 100.0f, 0.0f};		
	GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat lightDiffuse[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat lightSpecular[] = {0.9f, 0.9f, 0.9f, 1.0f};
	light = new Light(lightPos, lightAmbient, lightDiffuse, lightSpecular);

	//a toy plane model
	model = new Model(textureShader);
	model->DEBUG = false;
	model->loadFromFBX("../Project-3/toyplane.fbx");
	model->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	model->setLight(light);
	model->loadBitmap("../Project-3/bluecamo.bmp");
	model->loadVBO();
	//We'll take two of these
	t = 0;
	WorldActor* a = new WorldActor(model, 1.0f, 0.0f, -10.0f, 0.45f, t);
	a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(0.0f);
	actors->push_back(a);

	t = 4.019997;
	a = new WorldActor(model, 28.177160f, 0.0f, -23.101339f, 0.45f, t);
	a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(0.0f);
	actors->push_back(a);
	// A rocking horse maodel
	model = new Model(textureShader);
	model->DEBUG = false;
	model->loadFromFBX("../Project-3/rockie.fbx");
	model->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	model->setLight(light);
	model->loadBitmap("../Project-3/redpaint.bmp");
	model->loadVBO();
	//two of these. Like how i got my code to tell me where to put them?
	t = 2.019999;
	a = new WorldActor(model, 25.053471f, 0.0f, 4.773692f, 0.45f, t);
	a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(0.0f);
	actors->push_back(a);

	t = 6.019995;
	a = new WorldActor(model, 1.530541f, 0.0f, -14.341633f, 0.45f, t);
	a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(0.0f);
	actors->push_back(a);
	//The train goes with the camera
	trainModel = new Model(textureShader);
	trainModel->DEBUG = false;
	trainModel->loadFromFBX("../Project-3/train.fbx");
	trainModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	trainModel->setLight(light);
	trainModel->loadBitmap("../Project-3/rh_tex.bmp");
	trainModel->loadVBO();

	// Racetrack
	shininess = 40.0f;
	printf("=============Racetrack===============\n");
	trackModel = new Model(trackShader);
	trackModel->DEBUG = false;
	trackModel->loadFromFBX("../Project-3/racetrack.fbx");
	trackModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	trackModel->setLight(light);
	trackModel->loadBitmap("../Project-3/slate.bmp");
	trackModel->loadVBO();
	
	// guardrail
	shininess = 100.0f;
	printf("=============Guardrail===============\n");
	railModel = new Model(textureShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/inner_rail.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/diamond_tex2.bmp");
	railModel->loadVBO();
	//instantiate two pieces of each guardrail
	a = new WorldActor(railModel, -15.7f, -1.7f, -10.0f, 1.28f, t);
	a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
	rails->push_back(a);

	a = new WorldActor(railModel, 18.7f, -1.7f, -10.0f, 1.28f, t);
	a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
	rails->push_back(a);

	// guardrail
	printf("=============Guardrail===============\n");
	railModel = new Model(textureShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/outer_rail.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/diamond_tex2.bmp");
	railModel->loadVBO();
	// two outer rails too
	a = new WorldActor(railModel, -15.5f, -1.7f, -10.0f, 1.17f, t);
	a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
	rails->push_back(a);

	a = new WorldActor(railModel, 18.9f, -1.7f, -10.0f, 1.17f, t);
	a->setXangle(0.0f); a->setYangle(M_PI); a->setZangle(0.0f);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
	rails->push_back(a);

	// backdrop
	shininess = 15.0f;
	printf("=============Backdrop===============\n");
	railModel = new Model(trackShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/billboard.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/cartoon_forest.bmp");
	railModel->loadVBO();

	a = new WorldActor(railModel, -19.0f, 30.0f, -60.0f, 10.0f, t);
	a->setXangle(M_PI/2); a->setYangle(M_PI/2); a->setZangle(M_PI);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
	rails->push_back(a);
	// more crazy backdrops
	printf("=============Backdrop===============\n");
	railModel = new Model(trackShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/billboard.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/forest.bmp");
	railModel->loadVBO();

	a = new WorldActor(railModel, -50.0f, 30.0f, -29.0f, 10.0f, t);
	a->setXangle(M_PI/2); a->setYangle(M_PI); a->setZangle(M_PI+M_PI/2);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
	rails->push_back(a);

	printf("=============Backdrop===============\n");
	railModel = new Model(trackShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/billboard.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/forest-a.bmp");
	railModel->loadVBO();

	a = new WorldActor(railModel, -50.0f, 30.0f, 33.21f, 10.0f, t);
	a->setXangle(M_PI/2); a->setYangle(M_PI); a->setZangle(M_PI+M_PI/2);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
	rails->push_back(a);

	printf("=============Backdrop===============\n");
	railModel = new Model(trackShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/billboard.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/forest-b.bmp");
	railModel->loadVBO();

	a = new WorldActor(railModel, -20.0f, 30.0f, 40.0f, 10.0f, t);
	a->setXangle(M_PI/2); a->setYangle(M_PI); a->setZangle(0.0f);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
	rails->push_back(a);

	
	printf("=============Backdrop===============\n");
	railModel = new Model(trackShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/billboard.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/cartoon_forest.bmp");
	railModel->loadVBO();

	a = new WorldActor(railModel, 40.0f, 30.0f, 40.0f, 10.0f, t);
	a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(-M_PI/2);
	a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
	rails->push_back(a);
	//the spectators started the whole creepy theme
	printf("=============Spectator - boy 1===============\n");
	railModel = new Model(billboardShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/spec2.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/boy1a.bmp");
	railModel->loadVBO();
	//I like these cuz i can use for loops to make them
	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 10; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -22.5f + i*10.0f + j*0.5f, 0.4f + j*1.4f, -30.0 - j*2.0f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(M_PI/2); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}

	for (int i = 0; i < 5; ++i){
		for (int j = 0; j < 6; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -37.5f - j*2.0f,  0.4f + j*1.4f, -30.0f + i*10.0f + j*0.5f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}

	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 10; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -22.5f + i*10.0f + j*0.5f, 0.4f + j*1.4f, 10.0 + j*2.0f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(-M_PI/2); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}
	//more cartoon boys
	printf("=============Spectator - boy 2===============\n");
	railModel = new Model(billboardShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/spec2.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/boy2a.bmp");
	railModel->loadVBO();

	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 10; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -25.0f + i*10.0f + j*0.5f, 0.4f + j*1.4f, -30.0 - j*2.0f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(M_PI/2); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}

	for (int i = 0; i < 5; ++i){
		for (int j = 0; j < 6; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -37.5f - j*2.0f,  0.4f + j*1.4f, -27.5f + i*10.0f + j*0.5f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}

	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 10; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -25.0f + i*10.0f + j*0.5f, 0.4f + j*1.4f, 10.0 + j*2.0f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(-M_PI/2); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}
	//boys everywhere
	printf("=============Spectator - boy 3===============\n");
	railModel = new Model(billboardShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/spec2.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/boy3a.bmp");
	railModel->loadVBO();

	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 10; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -27.5f + i*10.0f + j*0.5f, 0.4f + j*1.4f, -30.0 - j*2.0f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(M_PI/2); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}

	for (int i = 0; i < 5; ++i){
		for (int j = 0; j < 6; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -37.5f - j*2.0f,  0.4f + j*1.4f, -25.0f + i*10.0f + j*0.5f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}

	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 10; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -27.5f + i*10.0f + j*0.5f, 0.4f + j*1.4f, 10.0 + j*2.0f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(-M_PI/2); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}

	//this one was a weird file
	printf("=============Spectator - boy 4===============\n");
	railModel = new Model(billboardShader);
	railModel->DEBUG = false;
	railModel->loadFromFBX("../Project-3/spec2.fbx");
	railModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	railModel->setLight(light);
	railModel->loadBitmap("../Project-3/boy4a.bmp");
	railModel->loadVBO();

	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 10; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -30.0f + i*10.0f + j*0.5f, 0.4f + j*1.4f, -30.0 - j*2.0f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(M_PI/2); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}

	for (int i = 0; i < 5; ++i){
		for (int j = 0; j < 6; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -37.5f - j*2.0f,  0.4f + j*1.4f, -22.5f + i*10.0f + j*0.5f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(0.0f); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}

	for (int i = 0; i < 6; ++i){
		for (int j = 0; j < 10; ++j){
			t = (rand() % 65)/10;
			a = new WorldActor(railModel, -30.0f + i*10.0f + j*0.5f, 0.4f + j*1.4f, 10.0 + j*2.0f, 1.0f, t);
			a->setXangle(0.0f); a->setYangle(-M_PI/2); a->setZangle(0.0f);
			a->setVelX(0.0f); a->setVelY(0.0f); a->setVelZ(-0.1f);
			crowd->push_back(a);
		}
	}

	// this is not an airplane!
	printf ("========== PLANE ==================\n");
	planeModel = new Model(textureShader);
	planeModel->DEBUG = false;
	planeModel->loadFromFBX("../Project-3/plane.fbx");
	planeModel->setMaterial(grassmatAmbient, grassmatDiffuse, grassmatSpecular, grassshininess);
	planeModel->loadBitmap("../Project-3/grass.bmp");
	planeModel->setLight(light);	
	planeModel->loadVBO();

	// Skybox obviously
	printf ("========== SKYBOX ================\n");
	skybox = new Skybox(skyboxShader);
	skybox->DEBUG = false;
	skybox->loadFromFBX("../Project-3/cube.fbx");
	skybox->loadCubeMapImages();
	skybox->loadVBO();

	//Gotta have a dragon, right?
	shininess = 100.0f;
	printf("=============DRAGON===============\n");
	dragonModel = new Model(textureShader);
	dragonModel->DEBUG = false;
	dragonModel->loadFromFBX("../Project-3/dragon.fbx");
	dragonModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	dragonModel->setLight(light);
	dragonModel->loadBitmap("../Project-3/scales.bmp");
	dragonModel->loadVBO();

	//and a teapot
	printf("============TEAPOT===============\n");
	teapotModel = new Model(textureShader);
	teapotModel->DEBUG = false;
	teapotModel->loadFromFBX("../Project-3/teapot.fbx");
	teapotModel->setMaterial(matAmbient, matDiffuse, matSpecular, shininess);		
	teapotModel->setLight(light);
	teapotModel->loadBitmap("../Project-3/grey.bmp");
	teapotModel->loadVBO();

	// Set up the camera 
	fov = 60.0f;
	camera = new FPSCamera(fov, 1.0f, 1.0f, 1000.0f, false);
	
	zooming = false;
	oldMouseX = 400;
	zVel = xVel = 0.0f;
	maxSpeed = 0.2f;

	// Allocate memory for the matrices
	rotXMatrix = new GLfloat[16];
	rotYMatrix = new GLfloat[16];
	rotZMatrix = new GLfloat[16];
	rotMatrix = new GLfloat[16];
	transMatrix = new GLfloat[16];
	scaleMatrix = new GLfloat[16];
	tempMatrix = new GLfloat[16];
	tempMatrix2 = new GLfloat[16];

	glutMainLoop();
	return 0;
}

