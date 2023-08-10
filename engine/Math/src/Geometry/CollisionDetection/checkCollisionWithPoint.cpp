#include "checkCollision.h"

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/computeProjection.h"
#include "engine/Math/src/Geometry/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	CollisionType checkCollision(
		Vector3D* point,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties)
	{
		switch (otherGeometry->getType())
		{
		case GeometryPrimitiveType::BOX:
			return checkCollision(point, (BoxGeometry*)otherGeometry, otherGeometryTransform);

		case GeometryPrimitiveType::AXIS_ALIGNED_BOX:
			return checkCollision(point, (AxisAlignedBoxGeometry*)otherGeometry, otherGeometryTransform);

		case GeometryPrimitiveType::SPHERE:
			return checkCollision(point, (SphereGeometry*)otherGeometry, otherGeometryTransform);

		case GeometryPrimitiveType::CAPSULE:
			return checkCollision(point, (CapsuleGeometry*)otherGeometry, otherGeometryTransform);

		case GeometryPrimitiveType::TRUNCATED_PYRAMID:
			// TODO: return checkCollision(point, (TruncatedPyramidGeometry*)otherGeometry, otherGeometryTransform);

		case GeometryPrimitiveType::MESH:
			// TODO: return checkCollision(bpointox, boxTransform, (mitevox::Mesh*)otherGeometry, otherGeometryTransform);

		default:
			break;
		}
		// TODO: compute collisionProperties
		return CollisionType::NONE;
	}

	CollisionType checkCollision(
		Vector3D* point,
		BoxGeometry* box,
		GeometryTransform* boxTransform)
	{
		Vector3D boxPosition = box->transform.translation;
		boxTransform->applyTo(boxPosition);
		if (*point == boxPosition)
		{
			return CollisionType::INSCRIBTION_2_1;
		}

		TriangleGeometry3D tmpTriangle = box->getTrianglePositions(0);
		boxTransform->applyTo(tmpTriangle);
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBoxProjectionMin = 0.0f, tmpBoxProjectionMax = 0.0f;

		// Check box's local Z-axis
		tmpPositionMax = tmpTriangle.point1;
		tmpPositionMin = box->getVertexPosition(7);
		boxTransform->applyTo(tmpPositionMin);
		tmpBoxProjectionMin = tmpPositionMin * tmpNormal;
		tmpBoxProjectionMax = tmpPositionMax * tmpNormal;

		if (tmpBoxProjectionMax < tmpBoxProjectionMin)
		{
			std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
		}
		float pointProjection = *point * tmpNormal;
		if (tmpBoxProjectionMax < pointProjection ||
			tmpBoxProjectionMin > pointProjection)
		{
			return CollisionType::NONE;
		}

		// Check box's local X-axis
		tmpTriangle = box->getTrianglePositions(2);
		boxTransform->applyTo(tmpTriangle);
		tmpNormal = tmpTriangle.computeNormal();

		tmpPositionMax = tmpTriangle.point1;
		tmpPositionMin = box->getVertexPosition(0);
		boxTransform->applyTo(tmpPositionMin);
		tmpBoxProjectionMin = tmpPositionMin * tmpNormal;
		tmpBoxProjectionMax = tmpPositionMax * tmpNormal;

		if (tmpBoxProjectionMax < tmpBoxProjectionMin)
		{
			std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
		}
		pointProjection = *point * tmpNormal;
		if (tmpBoxProjectionMax < pointProjection ||
			tmpBoxProjectionMin > pointProjection)
		{
			return CollisionType::NONE;
		}

		// Check box1's local Y-axis
		tmpTriangle = box->getTrianglePositions(4);
		boxTransform->applyTo(tmpTriangle);
		tmpNormal = tmpTriangle.computeNormal();

		tmpPositionMax = tmpTriangle.point1;
		tmpPositionMin = box->getVertexPosition(0);
		boxTransform->applyTo(tmpPositionMin);
		tmpBoxProjectionMin = tmpPositionMin * tmpNormal;
		tmpBoxProjectionMax = tmpPositionMax * tmpNormal;

		if (tmpBoxProjectionMax < tmpBoxProjectionMin)
		{
			std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
		}
		pointProjection = *point * tmpNormal;
		if (tmpBoxProjectionMax < pointProjection ||
			tmpBoxProjectionMin > pointProjection)
		{
			return CollisionType::NONE;
		}

		return CollisionType::INSCRIBTION_2_1;
	}

	CollisionType checkCollision(
		Vector3D* point,
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform)
	{
		// TODO: optimize (currently redirects to another function)

		BoxGeometry box;
		box.halfSize = axisAlignedBox->halfSize;
		box.transform.translation = axisAlignedBox->position;
		return checkCollision(point, &box, axisAlignedBoxTransform);
	}

	CollisionType checkCollision(
		Vector3D* point,
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform)
	{
		Vector3D spherePosition = sphere->transform.translation;
		sphereTransform->applyTo(spherePosition);
		float sphereRadius = sphere->radius * sphere->transform.scale.x() * sphereTransform->scale.x();

		if (*point == spherePosition)
		{
			return CollisionType::INSCRIBTION_2_1;
		}

		float min = spherePosition.x() - sphereRadius;
		float max = spherePosition.x() + sphereRadius;
		if (point->x() < min ||
			point->x() > max)
		{
			return CollisionType::NONE;
		}

		min = spherePosition.y() - sphereRadius;
		max = spherePosition.y() + sphereRadius;
		if (point->y() < min ||
			point->y() > max)
		{
			return CollisionType::NONE;
		}

		min = spherePosition.z() - sphereRadius;
		max = spherePosition.z() + sphereRadius;
		if (point->z() < min ||
			point->z() > max)
		{
			return CollisionType::NONE;
		}

		return CollisionType::INSCRIBTION_2_1;
	}

	CollisionType checkCollision(
		Vector3D* point,
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform)
	{
		float actualCapsuleHalfHeight = capsule->halfHeight * capsule->transform.scale.x();
		Vector3D capsulePosition1 = capsule->transform.translation;
		Vector3D capsulePosition2 = capsule->transform.translation;
		capsulePosition1.y() += actualCapsuleHalfHeight;
		capsulePosition2.y() -= actualCapsuleHalfHeight;
		capsuleTransform->applyTo(capsulePosition1);
		capsuleTransform->applyTo(capsulePosition2);

		mathem::Vector3D closestCapsulePoint = computeClosestPointOnTheLine(capsulePosition1, capsulePosition2, *point);
		float capsuleRadius = capsule->radius * capsule->transform.scale.x() * capsuleTransform->scale.x();

		if (*point == closestCapsulePoint)
		{
			return CollisionType::INSCRIBTION_2_1;
		}

		float min = closestCapsulePoint.x() - capsuleRadius;
		float max = closestCapsulePoint.x() + capsuleRadius;
		if (point->x() < min ||
			point->x() > max)
		{
			return CollisionType::NONE;
		}

		min = closestCapsulePoint.y() - capsuleRadius;
		max = closestCapsulePoint.y() + capsuleRadius;
		if (point->y() < min ||
			point->y() > max)
		{
			return CollisionType::NONE;
		}

		min = closestCapsulePoint.z() - capsuleRadius;
		max = closestCapsulePoint.z() + capsuleRadius;
		if (point->z() < min ||
			point->z() > max)
		{
			return CollisionType::NONE;
		}

		return CollisionType::INSCRIBTION_2_1;
	}
}