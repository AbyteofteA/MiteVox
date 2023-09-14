#include "checkCollisionWithConvexHull.h"

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkDistanceAxis.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computeProjection.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computePenetration.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	CollisionType checkCollisionConvexHullVsBox(
		GeometryPrimitiveBase* convexHull,
		GeometryTransform* convexHullTransform,
		GeometryPrimitiveBase* boxGeometry,
		GeometryTransform* boxTransform,
		CollisionProperties* collisionProperties)
	{
		if (checkDistanceAxis(convexHull, convexHullTransform, boxGeometry, boxTransform) == CollisionType::NONE)
		{
			return CollisionType::NONE;
		}

		bool thereMayBeACollision = true;

		float tmpHullProjectionMin = 0.0f, tmpHullProjectionMax = 0.0f;
		float tmpBoxProjectionMin = 0.0f, tmpBoxProjectionMax = 0.0f;

		// Check hull vs box
		{
			size_t hullTrianglesCount = convexHull->getTrianglesCount();
			for (size_t i = 0; i < hullTrianglesCount; ++i)
			{
				if (thereMayBeACollision == false)
				{
					break;
				}
				TriangleGeometry3D tmpTriangle = convexHull->getTrianglePositions(i);
				convexHullTransform->applyTo(tmpTriangle);
				Vector3D tmpNormal = tmpTriangle.computeNormal();

				computeProjection(convexHull, convexHullTransform, &tmpNormal, &tmpHullProjectionMin, &tmpHullProjectionMax);
				computeProjection(boxGeometry, boxTransform, &tmpNormal, &tmpBoxProjectionMin, &tmpBoxProjectionMax);
				float penetration = computePenetration(tmpHullProjectionMin, tmpHullProjectionMax, tmpBoxProjectionMin, tmpBoxProjectionMax);
				if (penetration == 0.0f)
				{
					thereMayBeACollision = false;
					collisionProperties->penetrationDepth = 0.0f;
				}
				else
				{
					collisionProperties->recomputePenetrationAndNormal(penetration, tmpNormal, true);
				}
			}
		}

		TriangleGeometry3D tmpTriangle = boxGeometry->getTrianglePositions(0);
		boxTransform->applyTo(tmpTriangle);
		Vector3D tmpBoxPositionMax;
		Vector3D tmpBoxPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();

		// Check box's local Z-axis
		{
			tmpBoxPositionMax = tmpTriangle.getPoint1();
			tmpBoxPositionMin = boxGeometry->getVertexPosition(7);
			boxTransform->applyTo(tmpBoxPositionMin);
			tmpBoxProjectionMin = tmpBoxPositionMin * tmpNormal;
			tmpBoxProjectionMax = tmpBoxPositionMax * tmpNormal;

			if (tmpBoxProjectionMax < tmpBoxProjectionMin)
			{
				std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
			}
			computeProjection(convexHull, convexHullTransform, &tmpNormal, &tmpHullProjectionMin, &tmpHullProjectionMax);
			float penetration = computePenetration(tmpBoxProjectionMin, tmpBoxProjectionMax, tmpHullProjectionMin, tmpHullProjectionMax);
			if (penetration == 0.0f)
			{
				thereMayBeACollision = false;
				collisionProperties->penetrationDepth = 0.0f;
			}
			else
			{
				collisionProperties->recomputePenetrationAndNormal(penetration, tmpNormal, false);
			}
		}

		// Check box's local X-axis
		if (thereMayBeACollision)
		{
			tmpTriangle = boxGeometry->getTrianglePositions(2);
			boxTransform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpBoxPositionMax = tmpTriangle.getPoint1();
			tmpBoxPositionMin = boxGeometry->getVertexPosition(0);
			boxTransform->applyTo(tmpBoxPositionMin);
			tmpBoxProjectionMin = tmpBoxPositionMin * tmpNormal;
			tmpBoxProjectionMax = tmpBoxPositionMax * tmpNormal;

			if (tmpBoxProjectionMax < tmpBoxProjectionMin)
			{
				std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
			}
			computeProjection(convexHull, convexHullTransform, &tmpNormal, &tmpHullProjectionMin, &tmpHullProjectionMax);
			float penetration = computePenetration(tmpBoxProjectionMin, tmpBoxProjectionMax, tmpHullProjectionMin, tmpHullProjectionMax);
			if (penetration == 0.0f)
			{
				thereMayBeACollision = false;
				collisionProperties->penetrationDepth = 0.0f;
			}
			else
			{
				collisionProperties->recomputePenetrationAndNormal(penetration, tmpNormal, false);
			}
		}

		// Check box's local Y-axis
		if (thereMayBeACollision)
		{
			tmpTriangle = boxGeometry->getTrianglePositions(4);
			boxTransform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpBoxPositionMax = tmpTriangle.getPoint1();
			tmpBoxPositionMin = boxGeometry->getVertexPosition(0);
			boxTransform->applyTo(tmpBoxPositionMin);
			tmpBoxProjectionMin = tmpBoxPositionMin * tmpNormal;
			tmpBoxProjectionMax = tmpBoxPositionMax * tmpNormal;

			if (tmpBoxProjectionMax < tmpBoxProjectionMin)
			{
				std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
			}
			computeProjection(convexHull, convexHullTransform, &tmpNormal, &tmpHullProjectionMin, &tmpHullProjectionMax);
			float penetration = computePenetration(tmpBoxProjectionMin, tmpBoxProjectionMax, tmpHullProjectionMin, tmpHullProjectionMax);
			if (penetration == 0.0f)
			{
				thereMayBeACollision = false;
				collisionProperties->penetrationDepth = 0.0f;
			}
			else
			{
				collisionProperties->recomputePenetrationAndNormal(penetration, tmpNormal, false);
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

	CollisionType checkCollisionConvexHullVsAABB(
		GeometryPrimitiveBase* convexHull,
		GeometryTransform* convexHullTransform,
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		CollisionProperties* collisionProperties)
	{
		// TODO: optimize (currently redirects to another function)

		GeometryPrimitiveBase box = axisAlignedBox->axisAlignedBoxToBox();
		return checkCollisionConvexHullVsBox(convexHull, convexHullTransform, &box, axisAlignedBoxTransform, collisionProperties);
	}

	CollisionType checkCollisionConvexHullVsTruncatedPyramid(
		GeometryPrimitiveBase* convexHull,
		GeometryTransform* convexHullTransform,
		GeometryPrimitiveBase* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionProperties* collisionProperties)
	{
		// Should work the same as with a box
		return checkCollisionConvexHullVsBox(convexHull, convexHullTransform, truncatedPyramid, truncatedPyramidTransform, collisionProperties);
	}

	CollisionType checkCollisionConvexHullVsConvexHullHelper(
		GeometryPrimitiveBase* convexHull1,
		GeometryTransform* convexHullTransform1,
		GeometryPrimitiveBase* convexHull2,
		GeometryTransform* convexHullTransform2,
		CollisionProperties* collisionProperties,
		bool isFirstPass = true)
	{
		if (checkDistanceAxis(convexHull1, convexHullTransform1, convexHull2, convexHullTransform2) == CollisionType::NONE)
		{
			return CollisionType::NONE;
		}

		bool thereMayBeACollision = true;

		float tmpHull1ProjectionMin = 0.0f, tmpHull1ProjectionMax = 0.0f;
		float tmpHull2ProjectionMin = 0.0f, tmpHull2ProjectionMax = 0.0f;

		size_t hull1TrianglesCount = convexHull1->getTrianglesCount();
		for (size_t i = 0; i < hull1TrianglesCount; ++i)
		{
			if (thereMayBeACollision == false)
			{
				break;
			}
			TriangleGeometry3D tmpTriangle = convexHull1->getTrianglePositions(i);
			convexHullTransform1->applyTo(tmpTriangle);
			Vector3D tmpNormal = tmpTriangle.computeNormal();

			computeProjection(convexHull1, convexHullTransform1, &tmpNormal, &tmpHull1ProjectionMin, &tmpHull1ProjectionMax);
			computeProjection(convexHull2, convexHullTransform2, &tmpNormal, &tmpHull2ProjectionMin, &tmpHull2ProjectionMax);
			float penetration = computePenetration(tmpHull1ProjectionMin, tmpHull1ProjectionMax, tmpHull2ProjectionMin, tmpHull2ProjectionMax);
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
			checkCollisionConvexHullVsConvexHullHelper(convexHull2, convexHullTransform2, convexHull1, convexHullTransform1, collisionProperties, false);
		}
		else
		{
			collisionProperties->type = CollisionType::INTERSECTION;
		}
	}

	CollisionType checkCollisionConvexHullVsConvexHull(
		GeometryPrimitiveBase* convexHull1,
		GeometryTransform* convexHullTransform1,
		GeometryPrimitiveBase* convexHull2,
		GeometryTransform* convexHullTransform2,
		CollisionProperties* collisionProperties)
	{
		return checkCollisionConvexHullVsConvexHullHelper(convexHull1, convexHullTransform1, convexHull2, convexHullTransform2, collisionProperties);
	}
}