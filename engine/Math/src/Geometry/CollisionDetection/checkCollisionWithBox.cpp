#include "checkCollision.h"

#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/computeProjection.h"
#include "engine/Math/src/Geometry/computePenetration.h"
#include "engine/Math/src/Geometry/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	/// checkCollision - BOX vs BOX
	/// 
	/// Algorithm:
	/// 1. Get (transformed) triangles 0, 2, 4 from box1
	/// 2. Extract normals from the triangles
	/// 3. Compute the box1's & box2's projections onto the normals (via dot product)
	/// 4. Check if the projections overlap
	CollisionType checkCollision(
		BoxGeometry* box1,
		GeometryTransform* box1Transform,
		BoxGeometry* box2,
		GeometryTransform* box2Transform,
		CollisionProperties* collisionProperties,
		bool isFirstPass = true);

	CollisionType checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		CollisionProperties* collisionProperties,
		bool isFirstPass = true);

	CollisionType checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		TruncatedPyramidGeometry* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionProperties* collisionProperties);

	// TODO: checkCollision BOX vs MESH
	// TODO: checkCollision BOX vs RAY

	CollisionType checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties)
	{
		switch (otherGeometry->getType())
		{
		case GeometryPrimitiveType::BOX:
			return checkCollision(box, boxTransform, (BoxGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::AXIS_ALIGNED_BOX:
			return checkCollision(box, boxTransform, (AxisAlignedBoxGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::SPHERE:
			return checkCollision(box, boxTransform, (SphereGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::CAPSULE:
			return checkCollision(box, boxTransform, (CapsuleGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::TRUNCATED_PYRAMID:
			return checkCollision(box, boxTransform, (TruncatedPyramidGeometry*)otherGeometry, otherGeometryTransform, collisionProperties);

		case GeometryPrimitiveType::MESH:
			// TODO: return checkCollision(box, boxTransform, (mitevox::Mesh*)otherGeometry, otherGeometryTransform, collisionInfo);

		default:
			break;
		}
		return CollisionType::NONE;
	}

	CollisionType checkCollision(
		BoxGeometry* box1,
		GeometryTransform* box1Transform,
		BoxGeometry* box2,
		GeometryTransform* box2Transform,
		CollisionProperties* collisionProperties,
		bool isFirstPass)
	{
		bool thereMayBeACollision = true;

		TriangleGeometry3D tmpTriangle = box1->getTrianglePositions(0);
		box1Transform->applyTo(tmpTriangle);
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBox1ProjectionMin = 0.0f, tmpBox1ProjectionMax = 0.0f;
		float tmpBox2ProjectionMin = 0.0f, tmpBox2ProjectionMax = 0.0f;

		// Check box1's local Z-axis
		{
			tmpPositionMax = tmpTriangle.point1;
			tmpPositionMin = box1->getVertexPosition(7);
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
			tmpTriangle = box1->getTrianglePositions(8);
			box1Transform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMax = tmpTriangle.point1;
			tmpPositionMin = box1->getVertexPosition(3);
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
			tmpTriangle = box1->getTrianglePositions(4);
			box1Transform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMax = tmpTriangle.point1;
			tmpPositionMin = box1->getVertexPosition(0);
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
			checkCollision(box2, box2Transform, box1, box1Transform, collisionProperties, false);
		}
		else
		{
			collisionProperties->type = CollisionType::INTERSECTION;
		}
		return collisionProperties->type;
	}

	CollisionType checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		CollisionProperties* collisionProperties,
		bool isFirstPass)
	{
		// TODO: optimize (currently redirects to another function)

		BoxGeometry box2;
		box2.halfSize = axisAlignedBox->halfSize;
		box2.transform.translation = axisAlignedBox->position;
		return checkCollision(box, boxTransform, &box2, axisAlignedBoxTransform, collisionProperties);
	}

	CollisionType checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform,
		CollisionProperties* collisionProperties)
	{
		bool thereMayBeACollision = true;

		TriangleGeometry3D tmpTriangle = box->getTrianglePositions(0);
		boxTransform->applyTo(tmpTriangle);
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBoxProjectionMin = 0.0f, tmpBoxProjectionMax = 0.0f;
		float tmpSphereProjectionMin = 0.0f, tmpSphereProjectionMax = 0.0f;

		// Check box's local Z-axis
		{
			tmpPositionMax = tmpTriangle.point1;
			tmpPositionMin = box->getVertexPosition(7);
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
			tmpNormal = box->transform.translation - sphere->transform.translation;
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

	CollisionType checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform,
		CollisionProperties* collisionProperties)
	{
		bool thereMayBeACollision = true;

		TriangleGeometry3D tmpTriangle = box->getTrianglePositions(0);
		boxTransform->applyTo(tmpTriangle);
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBoxProjectionMin = 0.0f, tmpBoxProjectionMax = 0.0f;
		float tmpCapsuleProjectionMin = 0.0f, tmpCapsuleProjectionMax = 0.0f;

		// Check box's local Z-axis
		{
			tmpPositionMax = tmpTriangle.point1;
			tmpPositionMin = box->getVertexPosition(7);
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
			computeProjection(capsule, capsuleTransform, &tmpNormal, &tmpCapsuleProjectionMin, &tmpCapsuleProjectionMax);
			if (tmpBoxProjectionMax < tmpCapsuleProjectionMin ||
				tmpCapsuleProjectionMax < tmpBoxProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check capsule vs box
		float actualCapsuleHalfHeight = capsule->halfHeight * capsule->transform.scale.x();
		Vector3D capsulePosition1 = capsule->transform.translation;
		Vector3D capsulePosition2 = capsule->transform.translation;
		capsulePosition1.y() += actualCapsuleHalfHeight;
		capsulePosition2.y() -= actualCapsuleHalfHeight;
		capsuleTransform->applyTo(capsulePosition1);
		capsuleTransform->applyTo(capsulePosition2);

		mathem::GeometryTransform zeroTransform;
		zeroTransform.reset();

		uint32_t  boxPointsCount = box->getVertecesCount();
		for (uint32_t i = 0; i < boxPointsCount; ++i)
		{
			Vector3D boxPoint = box->getVertexPosition(i);
			boxTransform->applyTo(boxPoint);

			Vector3D closestCapsuleSpherePosition =
				computeClosestPointOnTheLine(capsulePosition1, capsulePosition2, boxPoint);

			SphereGeometry closestSphere;
			float actualCapsuleRadius = capsule->radius * capsule->transform.scale.x() * capsuleTransform->scale.x();
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

	CollisionType checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		TruncatedPyramidGeometry* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionProperties* collisionProperties)
	{
		bool thereMayBeACollision = true;

		TriangleGeometry3D tmpTriangle = box->getTrianglePositions(0);
		boxTransform->applyTo(tmpTriangle);
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBox1ProjectionMin = 0.0f, tmpBox1ProjectionMax = 0.0f;
		float tmpBox2ProjectionMin = 0.0f, tmpBox2ProjectionMax = 0.0f;

		// Check box's local Z-axis
		{
			tmpPositionMin = box->getVertexPosition(7);
			tmpPositionMax = box->getVertexPosition(0);
			boxTransform->applyTo(tmpPositionMin);
			boxTransform->applyTo(tmpPositionMax);
			tmpBox1ProjectionMin = tmpPositionMin * tmpNormal;
			tmpBox1ProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(truncatedPyramid, truncatedPyramidTransform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check box's local X-axis
		if (thereMayBeACollision)
		{
			tmpTriangle = box->getTrianglePositions(2);
			boxTransform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMin = box->getVertexPosition(0);
			tmpPositionMax = box->getVertexPosition(3);
			boxTransform->applyTo(tmpPositionMin);
			boxTransform->applyTo(tmpPositionMax);
			tmpBox1ProjectionMin = tmpPositionMin * tmpNormal;
			tmpBox1ProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(truncatedPyramid, truncatedPyramidTransform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check box's local Y-axis
		if (thereMayBeACollision)
		{
			tmpTriangle = box->getTrianglePositions(4);
			boxTransform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMin = box->getVertexPosition(0);
			tmpPositionMax = box->getVertexPosition(1);
			boxTransform->applyTo(tmpPositionMin);
			boxTransform->applyTo(tmpPositionMax);
			tmpBox1ProjectionMin = tmpPositionMin * tmpNormal;
			tmpBox1ProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(truncatedPyramid, truncatedPyramidTransform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check pyramid's local Z-axis
		if (thereMayBeACollision)
		{
			tmpTriangle = truncatedPyramid->getTrianglePositions(0);
			truncatedPyramidTransform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMin = truncatedPyramid->getVertexPosition(7);
			tmpPositionMax = truncatedPyramid->getVertexPosition(0);
			truncatedPyramidTransform->applyTo(tmpPositionMin);
			truncatedPyramidTransform->applyTo(tmpPositionMax);
			tmpBox1ProjectionMin = tmpPositionMin * tmpNormal;
			tmpBox1ProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(box, boxTransform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check pyramid's local X-axis
		if (thereMayBeACollision)
		{
			tmpTriangle = truncatedPyramid->getTrianglePositions(2);
			truncatedPyramidTransform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMin = truncatedPyramid->getVertexPosition(0);
			tmpPositionMax = truncatedPyramid->getVertexPosition(3);
			truncatedPyramidTransform->applyTo(tmpPositionMin);
			truncatedPyramidTransform->applyTo(tmpPositionMax);
			tmpBox1ProjectionMin = tmpPositionMin * tmpNormal;
			tmpBox1ProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(box, boxTransform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check pyramid's local Y-axis
		if (thereMayBeACollision)
		{
			tmpTriangle = truncatedPyramid->getTrianglePositions(4);
			truncatedPyramidTransform->applyTo(tmpTriangle);
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMin = truncatedPyramid->getVertexPosition(0);
			tmpPositionMax = truncatedPyramid->getVertexPosition(1);
			truncatedPyramidTransform->applyTo(tmpPositionMin);
			truncatedPyramidTransform->applyTo(tmpPositionMax);
			tmpBox1ProjectionMin = tmpPositionMin * tmpNormal;
			tmpBox1ProjectionMax = tmpPositionMax * tmpNormal;

			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(box, boxTransform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
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
}