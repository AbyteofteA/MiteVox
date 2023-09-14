#include "checkCollisionWithPoint.h"

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computeProjection.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	CollisionType checkCollisionWithPoint(
		Vector3D point,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform)
	{
		switch (otherGeometry->getType())
		{
		case GeometryPrimitiveType::BOX:
			return checkCollisionPointVsBox(point, otherGeometry, otherGeometryTransform);

		case GeometryPrimitiveType::AXIS_ALIGNED_BOX:
			return checkCollisionPointVsAABB(point, otherGeometry, otherGeometryTransform);

		case GeometryPrimitiveType::SPHERE:
			return checkCollisionPointVsSphere(point, otherGeometry, otherGeometryTransform);

		case GeometryPrimitiveType::CAPSULE:
			return checkCollisionPointVsCapsule(point, otherGeometry, otherGeometryTransform);

		case GeometryPrimitiveType::TRUNCATED_PYRAMID:
			// TODO: return checkCollision(point, (TruncatedPyramidGeometry*)otherGeometry, otherGeometryTransform);

		default:
			break;
		}
		
		return CollisionType::NONE;
	}

	CollisionType checkCollisionWithPoint(
		GeometryPrimitiveBase* point,
		GeometryTransform* pointTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties)
	{
		Vector3D pointPosition = point->getPoint()->position;
		pointTransform->applyTo(pointPosition);
		// TODO: compute collisionProperties
		return checkCollisionWithPoint(pointPosition, otherGeometry, otherGeometryTransform);
	}

	CollisionType checkCollisionPointVsBox(
		Vector3D point,
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform)
	{
		BoxGeometry* boxGeometry = box->getBox();
		Vector3D boxPosition = boxGeometry->transform.translation;
		boxTransform->applyTo(boxPosition);
		if (point == boxPosition)
		{
			return CollisionType::INSCRIBTION_2_1;
		}

		TriangleGeometry3D tmpTriangle = boxGeometry->getTrianglePositions(0);
		boxTransform->applyTo(tmpTriangle);
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBoxProjectionMin = 0.0f, tmpBoxProjectionMax = 0.0f;

		// Check box's local Z-axis
		tmpPositionMax = tmpTriangle.getPoint1();
		tmpPositionMin = boxGeometry->getVertexPosition(7);
		boxTransform->applyTo(tmpPositionMin);
		tmpBoxProjectionMin = tmpPositionMin * tmpNormal;
		tmpBoxProjectionMax = tmpPositionMax * tmpNormal;

		if (tmpBoxProjectionMax < tmpBoxProjectionMin)
		{
			std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
		}
		float pointProjection = point * tmpNormal;
		if (tmpBoxProjectionMax < pointProjection ||
			tmpBoxProjectionMin > pointProjection)
		{
			return CollisionType::NONE;
		}

		// Check box's local X-axis
		tmpTriangle = boxGeometry->getTrianglePositions(2);
		boxTransform->applyTo(tmpTriangle);
		tmpNormal = tmpTriangle.computeNormal();

		tmpPositionMax = tmpTriangle.getPoint1();
		tmpPositionMin = boxGeometry->getVertexPosition(0);
		boxTransform->applyTo(tmpPositionMin);
		tmpBoxProjectionMin = tmpPositionMin * tmpNormal;
		tmpBoxProjectionMax = tmpPositionMax * tmpNormal;

		if (tmpBoxProjectionMax < tmpBoxProjectionMin)
		{
			std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
		}
		pointProjection = point * tmpNormal;
		if (tmpBoxProjectionMax < pointProjection ||
			tmpBoxProjectionMin > pointProjection)
		{
			return CollisionType::NONE;
		}

		// Check box1's local Y-axis
		tmpTriangle = boxGeometry->getTrianglePositions(4);
		boxTransform->applyTo(tmpTriangle);
		tmpNormal = tmpTriangle.computeNormal();

		tmpPositionMax = tmpTriangle.getPoint1();
		tmpPositionMin = boxGeometry->getVertexPosition(0);
		boxTransform->applyTo(tmpPositionMin);
		tmpBoxProjectionMin = tmpPositionMin * tmpNormal;
		tmpBoxProjectionMax = tmpPositionMax * tmpNormal;

		if (tmpBoxProjectionMax < tmpBoxProjectionMin)
		{
			std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
		}
		pointProjection = point * tmpNormal;
		if (tmpBoxProjectionMax < pointProjection ||
			tmpBoxProjectionMin > pointProjection)
		{
			return CollisionType::NONE;
		}

		return CollisionType::INSCRIBTION_2_1;
	}

	CollisionType checkCollisionPointVsBox(
		GeometryPrimitiveBase* point,
		GeometryTransform* pointTransform,
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		CollisionProperties* collisionProperties)
	{
		Vector3D pointPosition = point->getPoint()->position;
		pointTransform->applyTo(pointPosition);
		// TODO: compute collisionProperties
		return checkCollisionPointVsBox(pointPosition, box, boxTransform);
	}

	CollisionType checkCollisionPointVsAABB(
		Vector3D point,
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform)
	{
		// TODO: optimize (currently redirects to another function)

		GeometryPrimitiveBase box = axisAlignedBox->axisAlignedBoxToBox();
		return checkCollisionPointVsBox(point, &box, axisAlignedBoxTransform);
	}

	CollisionType checkCollisionPointVsAABB(
		GeometryPrimitiveBase* point,
		GeometryTransform* pointTransform,
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		CollisionProperties* collisionProperties)
	{
		Vector3D pointPosition = point->getPoint()->position;
		pointTransform->applyTo(pointPosition);
		// TODO: compute collisionProperties
		return checkCollisionPointVsAABB(pointPosition, axisAlignedBox, axisAlignedBoxTransform);
	}

	CollisionType checkCollisionPointVsSphere(
		Vector3D point,
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform)
	{
		SphereGeometry* sphereGeometry = sphere->getSphere();
		Vector3D spherePosition = sphereGeometry->transform.translation;
		sphereTransform->applyTo(spherePosition);
		float sphereRadius = sphereGeometry->radius * sphereGeometry->transform.scale.x() * sphereTransform->scale.x();

		if (point == spherePosition)
		{
			return CollisionType::INSCRIBTION_2_1;
		}

		float min = spherePosition.x() - sphereRadius;
		float max = spherePosition.x() + sphereRadius;
		if (point.x() < min ||
			point.x() > max)
		{
			return CollisionType::NONE;
		}

		min = spherePosition.y() - sphereRadius;
		max = spherePosition.y() + sphereRadius;
		if (point.y() < min ||
			point.y() > max)
		{
			return CollisionType::NONE;
		}

		min = spherePosition.z() - sphereRadius;
		max = spherePosition.z() + sphereRadius;
		if (point.z() < min ||
			point.z() > max)
		{
			return CollisionType::NONE;
		}

		return CollisionType::INSCRIBTION_2_1;
	}

	CollisionType checkCollisionPointVsSphere(
		GeometryPrimitiveBase* point,
		GeometryTransform* pointTransform,
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform,
		CollisionProperties* collisionProperties)
	{
		Vector3D pointPosition = point->getPoint()->position;
		pointTransform->applyTo(pointPosition);
		// TODO: compute collisionProperties
		return checkCollisionPointVsSphere(pointPosition, sphere, sphereTransform);
	}

	CollisionType checkCollisionPointVsCapsule(
		Vector3D point,
		GeometryPrimitiveBase* capsule,
		GeometryTransform* capsuleTransform)
	{
		CapsuleGeometry* capsuleGeometry = capsule->getCapsule();

		float actualCapsuleHalfHeight = capsuleGeometry->halfHeight * capsuleGeometry->transform.scale.x();
		Vector3D capsulePosition1 = capsuleGeometry->transform.translation;
		Vector3D capsulePosition2 = capsuleGeometry->transform.translation;
		capsulePosition1.y() += actualCapsuleHalfHeight;
		capsulePosition2.y() -= actualCapsuleHalfHeight;
		capsuleTransform->applyTo(capsulePosition1);
		capsuleTransform->applyTo(capsulePosition2);

		mathem::Vector3D closestCapsulePoint = computeClosestPointOnTheLine(capsulePosition1, capsulePosition2, point);
		float capsuleRadius = capsuleGeometry->radius * capsuleGeometry->transform.scale.x() * capsuleTransform->scale.x();

		if (point == closestCapsulePoint)
		{
			return CollisionType::INSCRIBTION_2_1;
		}

		float min = closestCapsulePoint.x() - capsuleRadius;
		float max = closestCapsulePoint.x() + capsuleRadius;
		if (point.x() < min ||
			point.x() > max)
		{
			return CollisionType::NONE;
		}

		min = closestCapsulePoint.y() - capsuleRadius;
		max = closestCapsulePoint.y() + capsuleRadius;
		if (point.y() < min ||
			point.y() > max)
		{
			return CollisionType::NONE;
		}

		min = closestCapsulePoint.z() - capsuleRadius;
		max = closestCapsulePoint.z() + capsuleRadius;
		if (point.z() < min ||
			point.z() > max)
		{
			return CollisionType::NONE;
		}

		return CollisionType::INSCRIBTION_2_1;
	}

	CollisionType checkCollisionPointVsCapsule(
		GeometryPrimitiveBase* point,
		GeometryTransform* pointTransform,
		GeometryPrimitiveBase* capsule,
		GeometryTransform* capsuleTransform,
		CollisionProperties* collisionProperties)
	{
		Vector3D pointPosition = point->getPoint()->position;
		pointTransform->applyTo(pointPosition);
		// TODO: compute collisionProperties
		return checkCollisionPointVsCapsule(pointPosition, capsule, capsuleTransform);
	}
}