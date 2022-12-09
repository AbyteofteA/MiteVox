#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionType.h"

#include <cstdint>

namespace mathem
{
	struct CollisionInfo
	{
		CollisionType type = CollisionType::NONE;

		GeometryPrimitiveBase* object1 = nullptr;
		GeometryPrimitiveBase* object2 = nullptr;

		mathem::Vector3D position;
		mathem::Vector3D normal;
		float penetrationDepth = 0.0f;
	};
}

#endif