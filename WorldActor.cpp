#include "WorldActor.h"



WorldActor::WorldActor(Model* m, float x, float y, float z, float s, float t){
	WorldActor::myModel = m;
	WorldActor::x = x;
	WorldActor::y = y;
	WorldActor::z = z;
	WorldActor::scale = s;
	WorldActor::theta = t;
	WorldActor::isAlive = true;
	WorldActor::direction = 1;
}

WorldActor::~WorldActor(void)
{
	delete WorldActor::myModel;
}
