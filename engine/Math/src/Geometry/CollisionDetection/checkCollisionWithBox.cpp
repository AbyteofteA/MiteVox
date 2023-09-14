#include "checkCollisionWithBox.h"

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkCollisionWithConvexHull.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkDistanceAxis.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computeProjection.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computePenetration.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	CollisionType checkCollisionBoxVsBoxHelper(
		GeometryPrimitiveBase* box1,
		GeometryTransform* box1Transform,
		GeometryPrimitiveBase* box2,
		GeometryTransform* box2Transform,
		CollisionProperties* collisionProperties,
		bool isFirstPass = true)
	{
		if (checkDistanceAxis(box1, box1Transform, box2, box2Transform) == CollisionType::NONE)
		{
			return CollisionType::NONE;
		}

		bool thereMayBeACollision = true;

		BoxGeometry* boxGeometry1 = box1->getBox();
		BoxGeometry* boxGeometry2 = box2->getBox();

		TriangleGeometry3D tmpTriangle = boxGeometry1->getTrianglePositions(0);
		box1Transform->applyTo(tmpTriangle);
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBox1ProjectionMin = 0.0f, tmpBox1ProjectionMax = 0.0f;
		float tmpBox2ProjectionMin = 0.0f, tmpBox2ProjectionMax = 0.0f;

		// Check box1's local Z-axis
		{
			tmpPositionMax = tmpTriangle.getPoint1();
			tmpPositionMin = boxGeometry1->getVertexPosition(7);
			box1Transform->applyTo(tmpPositionMin);
			tmpBox1ProjectionMin = tmpPositionMin * tmpNormal;
			tmpBox1ProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(box2, box2Transform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			float penetration = computePenetration(tmpBox1ProjectionMin, tmpBox1ProjectionMax, tmpBox2ProjectionMin, tmpBox2ProjectionMax);
			if (penetration == 0.0f)
			{
				thereMayBeACollision = false;
				collisionProperties->penetrationDepth = 0.0f;
			}
			else
			{
				collisionProperties->recomputePenetrationAndNormal(penetration, tmpNormal, isFirstPass);
			}
		}

		// Check box1's local X-axis
		if (thereMayBeACollision)
		{
			tmpTriangle = boxGeometry1->getTrianglePositions(8);
			box1Transform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMax = tmpTriangle.getPoint1();
			tmpPositionMin = boxGeometry1->getVertexPosition(3);
			box1Transform->applyTo(tmpPositionMin);
			tmpBox1ProjectionMin = tmpPositionMin * tmpNormal;
			tmpBox1ProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(box2, box2Transform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			float penetration = computePenetration(tmpBox1ProjectionMin, tmpBox1ProjectionMax, tmpBox2ProjectionMin, tmpBox2ProjectionMax);
			if (penetration == 0.0f)
			{
				thereMayBeACollision = false;
				collisionProperties->penetrationDepth = 0.0f;
			}
			else
			{
				collisionProperties->recomputePenetrationAndNormal(penetration, tmpNormal, isFirstPass);
			}
		}

		// Check box1's local Y-axis
		if (thereMayBeACollision)
		{
			tmpTriangle = boxGeometry1->getTrianglePositions(4);
			box1Transform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMax = tmpTriangle.getPoint1();
			tmpPositionMin = boxGeometry1->getVertexPosition(0);
			box1Transform->applyTo(tmpPositionMin);
			tmpBox1ProjectionMin = tmpPositionMin * tmpNormal;
			tmpBox1ProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(box2, box2Transform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			float penetration = computePenetration(tmpBox1ProjectionMin, tmpBox1ProjectionMax, tmpBox2ProjectionMin, tmpBox2ProjectionMax);
			if (penetration == 0.0f)
			{
				thereMayBeACollision = false;
				collisionProperties->penetrationDepth = 0.0f;
			}
			else
			{
				collisionProperties->recomputePenetrationAndNormal(penetration, tmpNormal, isFirstPass);
			}
		}

		if (thereMayBeACollision == false)
		{
			return CollisionType::NONE;
		}

		if (isFirstPass)
		{
			checkCollisionBoxVsBoxHelper(box2, box2Transform, box1, box1Transform, collisionProperties, false);
		}
		else
		{
			collisionProperties->type = CollisionType::INTERSECTION;
		}
		return collisionProperties->type;
	}

	CollisionType checkCollisionBoxVsBox(
		GeometryPrimitiveBase* box1,
		GeometryTransform* box1Transform,
		GeometryPrimitiveBase* box2,
		GeometryTransform* box2Transform,
		CollisionProperties* collisionProperties)
	{
		return checkCollisionBoxVsBoxHelper(box1, box1Transform, box2, box2Transform, collisionProperties);
	}

	CollisionType checkCollisionBoxVsAABB(
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		CollisionProperties* collisionProperties)
	{
		// TODO: optimize (currently redirects to another function)

		GeometryPrimitiveBase box2 = axisAlignedBox->axisAlignedBoxToBox();
		return checkCollisionBoxVsBox(box, boxTransform, &box2, axisAlignedBoxTransform, collisionProperties);
	}

	CollisionType checkCollisionBoxVsSphere(
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* sphere,
		GeometryTransform* sphereTransform,
		CollisionProperties* collisionProperties)
	{
		if (checkDistanceAxis(box, boxTransform, sphere, sphereTransform) == CollisionType::NONE)
		{
			return CollisionType::NONE;
		}

		bool thereMayBeACollision = true;

		BoxGeometry* boxGeometry = box->getBox();
		SphereGeometry* sphereGeometry = sphere->getSphere();

		TriangleGeometry3D tmpTriangle = boxGeometry->getTrianglePositions(0);
		boxTransform->applyTo(tmpTriangle);
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBoxProjectionMin = 0.0f, tmpBoxProjectionMax = 0.0f;
		float tmpSphereProjectionMin = 0.0f, tmpSphereProjectionMax = 0.0f;

		// Check box's local Z-axis
		{
			tmpPositionMax = tmpTriangle.getPoint1();
			tmpPositionMin = boxGeometry->getVertexPosition(7);
			boxTransform->applyTo(tmpPositionMin);
			tmpBoxProjectionMin = tmpPositionMin * tmpNormal;
			tmpBoxProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBoxProjectionMax < tmpBoxProjectionMin)
			{
				std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
			}
			computeProjection(sphere, sphereTransform, &tmpNormal, &tmpSphereProjectionMin, &tmpSphereProjectionMax);
			if (tmpBoxProjectionMax < tmpSphereProjectionMin ||
				tmpSphereProjectionMax < tmpBoxProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check box's local X-axis
		if (thereMayBeACollision)
		{
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
			computeProjection(sphere, sphereTransform, &tmpNormal, &tmpSphereProjectionMin, &tmpSphereProjectionMax);
			if (tmpBoxProjectionMax < tmpSphereProjectionMin ||
				tmpSphereProjectionMax < tmpBoxProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check box's local Y-axis
		if (thereMayBeACollision)
		{
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
			computeProjection(sphere, sphereTransform, &tmpNormal, &tmpSphereProjectionMin, &tmpSphereProjectionMax);
			if (tmpBoxProjectionMax < tmpSphereProjectionMin ||
				tmpSphereProjectionMax < tmpBoxProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check sphere vs box
		if (thereMayBeACollision)
		{
			tmpNormal = boxGeometry->transform.translation - sphereGeometry->transform.translation;
			tmpNormal.normalize();
			computeProjection(sphere, sphereTransform, &tmpNormal, &tmpSphereProjectionMin, &tmpSphereProjectionMax);
			computeProjection(box, boxTransform, &tmpNormal, &tmpBoxProjectionMin, &tmpBoxProjectionMax);
			if (tmpBoxProjectionMax < tmpSphereProjectionMin ||
				tmpSphereProjectionMax < tmpBoxProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		if (thereMayBeACollision == false)
		{
			return CollisionType::NONE;
		}

		collisionProperties->type = CollisionType::INTERSECTION;
		// TODO: compute collision properties
		return CollisionType::INTERSECTION;
	}

	CollisionType checkCollisionBoxVsCapsule(
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* capsule,
		GeometryTransform* capsuleTransform,
		CollisionProperties* collisionProperties)
	{
		if (checkDistanceAxis(box, boxTransform, capsule, capsuleTransform) == CollisionType::NONE)
		{
			return CollisionType::NONE;
		}

		bool thereMayBeACollision = true;

		BoxGeometry* boxGeometry = box->getBox();
		CapsuleGeometry* capsuleGeometry = capsule->getCapsule();

		TriangleGeometry3D tmpTriangle = boxGeometry->getTrianglePositions(0);
		boxTransform->applyTo(tmpTriangle);
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBoxProjectionMin = 0.0f, tmpBoxProjectionMax = 0.0f;
		float tmpCapsuleProjectionMin = 0.0f, tmpCapsuleProjectionMax = 0.0f;

		// Check box's local Z-axis
		{
			tmpPositionMax = tmpTriangle.getPoint1();
			tmpPositionMin = boxGeometry->getVertexPosition(7);
			boxTransform->applyTo(tmpPositionMin);
			tmpBoxProjectionMin = tmpPositionMin * tmpNormal;
			tmpBoxProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBoxProjectionMax < tmpBoxProjectionMin)
			{
				std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
			}
			computeProjection(capsule, capsuleTransform, &tmpNormal, &tmpCapsuleProjectionMin, &tmpCapsuleProjectionMax);
			if (tmpBoxProjectionMax < tmpCapsuleProjectionMin ||
				tmpCapsuleProjectionMax < tmpBoxProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check box's local X-axis
		if (thereMayBeACollision)
		{
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
			computeProjection(capsule, capsuleTransform, &tmpNormal, &tmpCapsuleProjectionMin, &tmpCapsuleProjectionMax);
			if (tmpBoxProjectionMax < tmpCapsuleProjectionMin ||
				tmpCapsuleProjectionMax < tmpBoxProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check box's local Y-axis
		if (thereMayBeACollision)
		{
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
			computeProjection(capsule, capsuleTransform, &tmpNormal, &tmpCapsuleProjectionMin, &tmpCapsuleProjectionMax);
			if (tmpBoxProjectionMax < tmpCapsuleProjectionMin ||
				tmpCapsuleProjectionMax < tmpBoxProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check capsule vs box
		float actualCapsuleHalfHeight = capsuleGeometry->halfHeight * capsuleGeometry->transform.scale.x();
		Vector3D capsulePosition1 = capsuleGeometry->transform.translation;
		Vector3D capsulePosition2 = capsuleGeometry->transform.translation;
		capsulePosition1.y() += actualCapsuleHalfHeight;
		capsulePosition2.y() -= actualCapsuleHalfHeight;
		capsuleTransform->applyTo(capsulePosition1);
		capsuleTransform->applyTo(capsulePosition2);

		mathem::GeometryTransform zeroTransform;
		zeroTransform.reset();

		uint32_t  boxPointsCount = boxGeometry->getVertecesCount();
		for (uint32_t i = 0; i < boxPointsCount; ++i)
		{
			Vector3D boxPoint = boxGeometry->getVertexPosition(i);
			boxTransform->applyTo(boxPoint);

			Vector3D closestCapsuleSpherePosition =
				computeClosestPointOnTheLine(capsulePosition1, capsulePosition2, boxPoint);

			SphereGeometry closestSphere;
			float actualCapsuleRadius = capsuleGeometry->radius * capsuleGeometry->transform.scale.x() * capsuleTransform->scale.x();
			closestSphere.radius = actualCapsuleRadius;
			closestSphere.transform.reset();
			closestSphere.transform.translation = closestCapsuleSpherePosition;

			tmpNormal = boxPoint - closestCapsuleSpherePosition;
			tmpNormal.normalize();
			computeProjection(&closestSphere, &zeroTransform, &tmpNormal, &tmpCapsuleProjectionMin, &tmpCapsuleProjectionMax);
			computeProjection(box, boxTransform, &tmpNormal, &tmpBoxProjectionMin, &tmpBoxProjectionMax);
			if (tmpBoxProjectionMax < tmpCapsuleProjectionMin ||
				tmpCapsuleProjectionMax < tmpBoxProjectionMin)
			{
				thereMayBeACollision = false;
				break;
			}
		}

		if (thereMayBeACollision == false)
		{
			return CollisionType::NONE;
		}

		collisionProperties->type = CollisionType::INTERSECTION;
		// TODO: compute collision properties
		return CollisionType::INTERSECTION;
	}

	CollisionType checkCollisionBoxVsTruncatedPyramid(
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionProperties* collisionProperties)
	{
		// Should work the same as with a box
		return checkCollisionBoxVsBox(box, boxTransform, truncatedPyramid, truncatedPyramidTransform, collisionProperties);
	}

	CollisionType checkCollisionBoxVsConvexHull(
		GeometryPrimitiveBase* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* convexHull,
		GeometryTransform* convexHullTransform,
		CollisionProperties* collisionProperties)
	{
		CollisionType result = checkCollisionConvexHullVsBox(convexHull, convexHullTransform, box, boxTransform, collisionProperties);
		collisionProperties->normalBelongsToTheFirst = !collisionProperties->normalBelongsToTheFirst;
		return result;
	}
}