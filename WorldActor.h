#ifndef ACTOR
#define ACTOR
#include "Model.h"
#include "MathHelper.h"

class WorldActor
{
	float velX, velY, velZ;
	float x, y, z;
	float scale;
	float xAngle, yAngle, zAngle;
	float theta;
	int direction;

public:
	WorldActor(Model* m, float x, float y, float z, float s, float t);
	~WorldActor(void);

	Model* myModel;
	bool isAlive;

	float getScale() {return scale;}
	void setScale(float s) {scale=s;}
	float getX() {return x;}
	void setX(float posx) {x = posx;}
	float getY() {return y;}
	void setY(float posy) {y = posy;}
	float getZ() {return z;}
	void setZ(float posz) {z = posz;}
	float getXangle() {return xAngle;}
	void setXangle(float xa) {xAngle = xa;}
	float getYangle() {return yAngle;}
	void setYangle(float ya) {yAngle = ya;}
	float getZangle() {return zAngle;}
	void setZangle(float za) {zAngle=za;}
	float getVelX() {return velX;}
	void setVelX(float vx) {velX=vx;}
	float getVelY() {return velY;}
	void setVelY(float vy) {velY=vy;}
	float getVelZ() {return velZ;}
	void setVelZ(float vz) {velZ=vz;}
	float getTheta() {return theta;}
	void setTheta(float t) {theta=t;}
	int getDirection() { return direction; }
	void toggleDirection() { direction = -direction; }
};

#endif