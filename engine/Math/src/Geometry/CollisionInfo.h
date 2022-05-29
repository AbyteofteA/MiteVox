#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H

#include "engine/Math/src/LinearAlgebra/Point3D.h"
#include "engine/Math/src/LinearAlgebra/Vector3D.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"

#include <cstdint>

namespace mathem
{
	enum struct CollisionType : uint8_t { NONE, INTERSECTION, INSCRIBTION_1_2, INSCRIBTION_2_1 };

	struct CollisionInfo
	{
		CollisionType type = CollisionType::NONE;

		GeometryPrimitiveBase* object1 = nullptr;
		GeometryPrimitiveBase* object2 = nullptr;

		mathem::Point3D position;
		mathem::Vector3D normal;
		float penetrationDepth = 0.0f;
	};
}

#endif