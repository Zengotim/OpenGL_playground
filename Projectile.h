#pragma once
#include "worldactor.h"
class Projectile :
	public WorldActor
{
public:
	Projectile(Model* m, float x, float y, float z, float s, float t);
	~Projectile(void);
	int age;
};

