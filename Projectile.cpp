#include "Projectile.h"
#include <math.h>

Projectile::Projectile(Model* m, float x, float y, float z, float s, float t) :WorldActor(m, x, y, z, s, t){
	Projectile::setVelY(0.3f);
	Projectile::age = 0;
}


Projectile::~Projectile(void)
{
	delete Projectile::myModel;
}
