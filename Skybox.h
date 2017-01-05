#ifndef SKYBOX
#define SKYBOX 
#include "Model.h"
#include "Shader.h"

class Skybox : public Model{
public:
	Skybox(Shader* shader);
	~Skybox();
	void loadCubeMapImages();
private:
	GLuint cubeMapID;
};
#endif