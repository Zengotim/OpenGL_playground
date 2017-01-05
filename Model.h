#ifndef MODEL
#define MODEL
#include "Shader.h"
#include "MathHelper.h"
#include "Light.h"
#include <fbxsdk.h> 

class Model {
public:
	Model(Shader* shader);		// Note, in C++, constructors/destructors aren't inherited
	~Model();
	void setGeometry(GLfloat vertices[], int size);
	void setNormals(GLfloat* normals, int size);
	void setVertexColors(GLfloat* colors, int size);
	void setTextureCoordinates(GLfloat* uvs, int size);
	void loadFromFBX(const char* filename);
	void setIndexBuffer(GLuint* indices, int size);	
	void setMaterial (GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat shininess);
	void loadBitmapFromFile(const char* filename, int* width, int* height, int* size, unsigned char** pixel_data);
	virtual void loadBitmap (const char* filename);
	virtual void loadBitmapWithAlpha(const char* filename);
	virtual void setLight (Light* light);
	
	void loadVBO();
	virtual void render();

	GLfloat*	mR;			// A rotation matrix (used in the shader to rotate normals)

	GLfloat*	mM;			// A matrix holding all orientation information for this model.
							// This will get passed off the mV variable in the shader
							// This is the "M" part of the MVP matrix
	Shader*		shader;		// The shader object for this model
	Light*		light;		// The *only* light for this object

	bool		DEBUG;
	
protected:
	
	GLfloat*	vertices;	// x, y, z
	GLfloat*	normals;	// Vertex normals
	GLfloat*	uvs;		// Texture coordinates
	GLfloat*	colors;		// Colors for each vertex
	GLuint*		indices;	// Index buffer							
	GLuint		numVertices, numNormals, numUVs, numColors, numIndices;
	GLuint		vao;		// Vertex Array Object
	GLuint		vbo;		// Vertex Buffer Object
	GLuint		index_vbo;	// Index Buffer Object
	GLuint		tex_buffer_ID;		// A texture buffer ID			// NEW!!

	GLfloat*	ambient, *combinedAmbient;	// Ambient color				
	GLfloat*	diffuse, *combinedDiffuse;	// Diffuse color				
	GLfloat*	specular, *combinedSpecular;	// Specular color				
	GLfloat		shininess;	// Shininess					

	GLuint		vPosition;	// A reference to the variable "vPosition" in the shader
	GLuint		vNormal;	// A refernece to the variable "vNormal" in the shader
	GLuint		vTexCoord;	// A reference to the variable "vTexCoord" in the shader
	GLuint		texture;	// A reference to the variable "texture" in the fragment shader
	GLuint		vColor;		// A reference to the variable "vColor" in the shader
	GLuint		umM;		// A reference to the variable "mM" in the shader
	GLuint		umR;		// A reference to the variable "mR" in the shader
	GLuint		ambientID;	// A reference to the ambient variable		
	GLuint		diffuseID;	// A reference to the diffuse variable		
	GLuint		specularID;	// A reference to the specular variable	
	GLuint		shineID;	// A reference to the shininess variable


	void traverseFBXNodes(KFbxNode* node);
};
#endif