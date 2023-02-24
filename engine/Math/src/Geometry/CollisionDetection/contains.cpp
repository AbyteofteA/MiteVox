#include "contains.h"

#include "checkCollision.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/SphereGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/CapsuleGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TruncatedPyramidGeometry.h"
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
				collisionType = checkCollision(&vertexPosition, (AxisAlignedBoxGeometry*)geometryPrimitiveContainer, geometryPrimitiveContainerTransform);
				break;

			case mathem::GeometryPrimitiveType::BOX:
				collisionType = checkCollision(&vertexPosition, (BoxGeometry*)geometryPrimitiveContainer, geometryPrimitiveContainerTransform);
				break;

			case mathem::GeometryPrimitiveType::SPHERE:
				collisionType = checkCollision(&vertexPosition, (SphereGeometry*)geometryPrimitiveContainer, geometryPrimitiveContainerTransform);
				break;

			case mathem::GeometryPrimitiveType::CAPSULE:
				collisionType = checkCollision(&vertexPosition, (CapsuleGeometry*)geometryPrimitiveContainer, geometryPrimitiveContainerTransform);
				break;

			case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
				// TODO:
				break;

			case mathem::GeometryPrimitiveType::MESH:
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
		size_t geometryPrimitivesCount = complexGeometry->primitives.getElementsCount();
		if (geometryPrimitivesCount == 0)
		{
			return false;
		}

		for (size_t i = 0; i < geometryPrimitivesCount; ++i)
		{
			GeometryPrimitiveBase* geometryPrimitive = complexGeometry->primitives.getElement(i);

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