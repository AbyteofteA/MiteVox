#include "checkCollision.h"

#include "engine/Math/src/Geometry/computeProjection.h"
#include "engine/Math/src/Geometry/computeClosestPointOnTheLine.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	bool checkCollision(
		BoxGeometry* box1,
		GeometryTransform* box1Transform,
		BoxGeometry* box2,
		GeometryTransform* box2Transform,
		CollisionInfo* collisionInfo,
		bool isFirstPass)
	{
		bool thereMayBeACollision = true;

		TriangleGeometry tmpTriangle = box1->getTrianglePositions(0);
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
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		// Check box1's local X-axis
		if (thereMayBeACollision)
		{
			tmpTriangle = box1->getTrianglePositions(2);
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
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				thereMayBeACollision = false;
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
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				thereMayBeACollision = false;
			}
		}

		if (thereMayBeACollision == false)
		{
			return false;
		}

		if (isFirstPass)
		{
			return checkCollision(box2, box2Transform, box1, box1Transform, collisionInfo, false);
		}
		else
		{
			if (collisionInfo)
			{
				collisionInfo->object1 = box1;
				collisionInfo->object2 = box2;
				collisionInfo->type = CollisionType::INTERSECTION;
				// TODO: compute collision info
			}
			return true;
		}
	}

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		AxisAlignedBoxGeometry* axisAlignedBox,
		CollisionInfo* collisionInfo,
		bool isFirstPass)
	{
		// TODO: optimize (currently redirects to another function)

		GeometryTransform* box2Transform = new GeometryTransform();
		BoxGeometry* box2 = new BoxGeometry();
		box2->halfSize = axisAlignedBox->halfSize;
		box2->transform.translation = axisAlignedBox->position;
		bool result = checkCollision(box, boxTransform, box2, box2Transform, collisionInfo);
		delete box2Transform;
		delete box2;
		return result;
	}

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform,
		CollisionInfo* collisionInfo)
	{
		bool thereMayBeACollision = true;

		TriangleGeometry tmpTriangle = box->getTrianglePositions(0);
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
				return false;
			}
		}

		if (thereMayBeACollision == false)
		{
			return false;
		}

		if (collisionInfo)
		{
			collisionInfo->object1 = box;
			collisionInfo->object2 = sphere;
			collisionInfo->type = CollisionType::INTERSECTION;
			// TODO: compute collision info
		}
		return true;
	}

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform,
		CollisionInfo* collisionInfo)
	{
		bool thereMayBeACollision = true;

		TriangleGeometry tmpTriangle = box->getTrianglePositions(0);
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
			return false;
		}

		if (collisionInfo)
		{
			collisionInfo->object1 = box;
			collisionInfo->object2 = capsule;
			collisionInfo->type = CollisionType::INTERSECTION;
			// TODO: compute collision info
		}
		return true;
	}

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		TruncatedPyramidGeometry* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionInfo* collisionInfo)
	{
		bool thereMayBeACollision = true;

		TriangleGeometry tmpTriangle = box->getTrianglePositions(0);
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
			return false;
		}

		if (collisionInfo)
		{
			collisionInfo->object1 = box;
			collisionInfo->object2 = truncatedPyramid;
			collisionInfo->type = CollisionType::INTERSECTION;
			// TODO: compute collision info
		}
		return true;
	}

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionInfo* collisionInfo)
	{
		Vector3D collisionPoint;

		switch (otherGeometry->getType())
		{
		case GeometryPrimitiveType::BOX:
		{
			BoxGeometry* otherBox = (BoxGeometry*)otherGeometry;
			Vector3D tmpCollisionPoint;
			return checkCollision(box, boxTransform, otherBox, otherGeometryTransform, collisionInfo);
		}

		case GeometryPrimitiveType::SPHERE:
		{
			SphereGeometry* otherSphere = (SphereGeometry*)otherGeometry;
			Vector3D tmpCollisionPoint;
			return checkCollision(box, boxTransform, otherSphere, otherGeometryTransform, collisionInfo);
		}

		case GeometryPrimitiveType::CAPSULE:
		{
			CapsuleGeometry* otherCapsule = (CapsuleGeometry*)otherGeometry;
			Vector3D tmpCollisionPoint;
			return checkCollision(box, boxTransform, otherCapsule, otherGeometryTransform, collisionInfo);
		}

		case GeometryPrimitiveType::TRUNCATED_PYRAMID:
		{
			TruncatedPyramidGeometry* otherTruncatedPyramid = (TruncatedPyramidGeometry*)otherGeometry;
			Vector3D tmpCollisionPoint;
			return checkCollision(box, boxTransform, otherTruncatedPyramid, otherGeometryTransform, collisionInfo);
		}

		case GeometryPrimitiveType::MESH:
		{
			mitevox::Mesh* otherMesh = (mitevox::Mesh*)otherGeometry;
			Vector3D tmpCollisionPoint;
			// TODO: return checkCollision(box, boxTransform, otherMesh, otherGeometryTransform, collisionInfo);
		}

		default:
			break;
		}
		return false;
	}
}