#include "checkCollision.h"

#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"

namespace mathem
{
	CollisionType checkCollision(
		ComplexGeometry* complexGeometry,
		GeometryTransform* complexGeometryTransform,
		AxisAlignedBoxGeometry* axisAlignedBox)
	{
		CollisionType collisionType = CollisionType::NONE;

		size_t geometryPrimitivesCount = complexGeometry->primitives.getElementsCount();
		for (size_t i = 0; i < geometryPrimitivesCount; ++i)
		{
			CollisionInfo primitiveCollisionInfo;

			GeometryPrimitiveBase* geometryPrimitive = complexGeometry->primitives.getElement(i);
			GeometryPrimitiveType geometryPrimitiveType = geometryPrimitive->getType();
			switch (geometryPrimitiveType)
			{
			case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
				checkCollision((AxisAlignedBoxGeometry*)geometryPrimitive, axisAlignedBox, &primitiveCollisionInfo);
				break;

			case mathem::GeometryPrimitiveType::BOX:
				checkCollision((BoxGeometry*)geometryPrimitive, complexGeometryTransform, axisAlignedBox, &primitiveCollisionInfo);
				break;

			case mathem::GeometryPrimitiveType::SPHERE:
				// TODO:
				break;

			case mathem::GeometryPrimitiveType::CAPSULE:
				// TODO:
				break;

			case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
				// TODO:
				break;

			case mathem::GeometryPrimitiveType::RAY:
				// TODO:
				break;

			case mathem::GeometryPrimitiveType::MESH:
				// TODO:
				break;

			default:
				continue;
			}

			collisionType = (mathem::CollisionType)((uint8_t)collisionType | (uint8_t)primitiveCollisionInfo.type);

			if (collisionType == mathem::CollisionType::INTERSECTION)
			{
				return collisionType;
			}
		}

		return collisionType;
	}
}