#ifndef SEPARATEOBJECTS_H
#define SEPARATEOBJECTS_H

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/MiteVox/src/Playground/Entity.h"

namespace mitevox
{
	void separateObjects(mathem::CollisionInfo<Entity*>* collisionInfo);
}

#endif