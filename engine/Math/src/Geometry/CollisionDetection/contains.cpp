#include "contains.h"

#include "checkCollision.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkCollisionWithPoint.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"

namespace mathem
{
	bool containsIdeal(
		GeometryPrimitiveBase* geometryPrimitive,
		GeometryTransform* geometryPrimitiveTransform,
		GeometryPrimitiveBase* geometryPrimitiveContainer,
		GeometryTransform* geometryPrimitiveContainerTransform)
	{
		GeometryPrimitiveType geometryPrimitiveType = geometryPrimitive->getType();
		switch (geometryPrimitiveType)
		{
		case mathem::GeometryPrimitiveType::SPHERE:
			// TODO:
			break;

		case mathem::GeometryPrimitiveType::CAPSULE:
			// TODO:
			break;

		default:
			break;
		}
		return false;
	}

	bool contains(
		GeometryPrimitiveBase* geometryPrimitive,
		GeometryTransform* geometryPrimitiveTransform,
		GeometryPrimitiveBase* geometryPrimitiveContainer,
		GeometryTransform* geometryPrimitiveContainerTransform)
	{
		GeometryPrimitiveType geometryPrimitiveContainerType = geometryPrimitiveContainer->getType();

		size_t geometryPrimitiveVertecesCount = geometryPrimitive->getVertecesCount();
		if (geometryPrimitiveVertecesCount == 0)
		{
			return containsIdeal(
				geometryPrimitive,
				geometryPrimitiveTransform,
				geometryPrimitiveContainer,
				geometryPrimitiveContainerTransform);
		}

		for (size_t i = 0; i < geometryPrimitiveVertecesCount; ++i)
		{
			Vector3D vertexPosition = geometryPrimitive->getVertexPosition(i);
			geometryPrimitiveTransform->applyTo(vertexPosition);

			CollisionType collisionType = CollisionType::NONE;
			switch (geometryPrimitiveContainerType)
			{
			case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
				collisionType = checkCollisionPointVsAABB(vertexPosition, geometryPrimitiveContainer, geometryPrimitiveContainerTransform);
				break;

			case mathem::GeometryPrimitiveType::BOX:
				collisionType = checkCollisionPointVsBox(vertexPosition, geometryPrimitiveContainer, geometryPrimitiveContainerTransform);
				break;

			case mathem::GeometryPrimitiveType::SPHERE:
				collisionType = checkCollisionPointVsSphere(vertexPosition, geometryPrimitiveContainer, geometryPrimitiveContainerTransform);
				break;

			case mathem::GeometryPrimitiveType::CAPSULE:
				collisionType = checkCollisionPointVsCapsule(vertexPosition, geometryPrimitiveContainer, geometryPrimitiveContainerTransform);
				break;

			case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
				// TODO:
				break;

			default:
				break;
			}

			if (collisionType != CollisionType::INSCRIBTION_2_1)
			{
				return false;
			}
		}

		return true;
	}

	bool contains(
		ComplexGeometry* complexGeometry,
		GeometryTransform* complexGeometryTransform,
		GeometryPrimitiveBase* geometryPrimitiveContainer,
		GeometryTransform* geometryPrimitiveContainerTransform)
	{
		if (complexGeometry->getType() == GeometryType::POINT)
		{
			Vector3D point = complexGeometryTransform->translation;
			CollisionProperties collisionProperties;
			CollisionType collisionType = checkCollisionWithPoint(point, geometryPrimitiveContainer, geometryPrimitiveContainerTransform);
			return collisionType == CollisionType::INSCRIBTION_2_1;
		}

		size_t geometryPrimitivesCount = complexGeometry->getPrimitivesCount();
		if (geometryPrimitivesCount == 0)
		{
			return false;
		}

		for (size_t i = 0; i < geometryPrimitivesCount; ++i)
		{
			GeometryPrimitiveBase* geometryPrimitive = complexGeometry->getPrimitive(i);

			bool containsPrimitive = contains(
				geometryPrimitive,
				complexGeometryTransform,
				geometryPrimitiveContainer,
				geometryPrimitiveContainerTransform);
			if (containsPrimitive == false)
			{
				return false;
			}
		}
		return true;
	}
}