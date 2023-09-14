#ifndef CHECKDISTANCEAXIS_H
#define CHECKDISTANCEAXIS_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionType.h"

namespace mathem
{
	CollisionType checkDistanceAxis(
		GeometryPrimitiveBase* object1,
		GeometryTransform* objectTransform1, 
		GeometryPrimitiveBase* object2,
		GeometryTransform* objectTransform2);
}

#endif