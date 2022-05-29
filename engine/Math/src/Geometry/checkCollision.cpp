#include "checkCollision.h"

#include "computeProjection.h"
#include "computeClosestPointOnTheLine.h"

namespace mathem
{
	bool checkCollision(
		BoxGeometry* box1,
		GeometryTransform* box1Transform,
		BoxGeometry* box2,
		GeometryTransform* box2Transform,
		bool computeCollisionInfo,
		CollisionInfo* collisionInfo,
		bool isFirstPass)
	{
		if (isFirstPass)
		{
			collisionInfo->object1 = box1;
			collisionInfo->object1 = box2;
			collisionInfo->type = CollisionType::INTERSECTION;
		}

		TriangleGeometry tmpTriangle = box1->getTriangle(0, box1Transform);
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBox1ProjectionMin = 0.0f, tmpBox1ProjectionMax = 0.0f;
		float tmpBox2ProjectionMin = 0.0f, tmpBox2ProjectionMax = 0.0f;

		// Check box1's local Z-axis
		{
			tmpBox1ProjectionMin = dotProduct(box1->getPoint(7, box1Transform), tmpNormal);
			tmpBox1ProjectionMax = dotProduct(box1->getPoint(0, box1Transform), tmpNormal);
			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(box2, box2Transform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		// Check box1's local X-axis
		{
			tmpTriangle = box1->getTriangle(2, box1Transform);
			tmpNormal = tmpTriangle.computeNormal();
			tmpBox1ProjectionMin = dotProduct(box1->getPoint(0, box1Transform), tmpNormal);
			tmpBox1ProjectionMax = dotProduct(box1->getPoint(3, box1Transform), tmpNormal);
			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(box2, box2Transform , &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		// Check box1's local Y-axis
		{
			tmpTriangle = box1->getTriangle(4, box1Transform);
			tmpNormal = tmpTriangle.computeNormal();
			tmpBox1ProjectionMin = dotProduct(box1->getPoint(0, box1Transform), tmpNormal);
			tmpBox1ProjectionMax = dotProduct(box1->getPoint(1, box1Transform), tmpNormal);
			if (tmpBox1ProjectionMax < tmpBox1ProjectionMin)
			{
				std::swap(tmpBox1ProjectionMin, tmpBox1ProjectionMax);
			}
			computeProjection(box2, box2Transform, &tmpNormal, &tmpBox2ProjectionMin, &tmpBox2ProjectionMax);
			if (tmpBox1ProjectionMax < tmpBox2ProjectionMin ||
				tmpBox2ProjectionMax < tmpBox1ProjectionMin)
			{
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		if (isFirstPass)
		{
			return checkCollision(box2, box2Transform, box1, box1Transform, computeCollisionInfo, collisionInfo, false);
		}
		else
		{
			if (computeCollisionInfo)
			{
				// TODO: compute collision info
			}
			return true;
		}
	}

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform, 
		bool computeCollisionInfo,
		CollisionInfo* collisionInfo)
	{
		TriangleGeometry tmpTriangle = box->getTriangle(0, boxTransform);
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBoxProjectionMin = 0.0f, tmpBoxProjectionMax = 0.0f;
		float tmpSphereProjectionMin = 0.0f, tmpSphereProjectionMax = 0.0f;

		// Check box's local Z-axis
		{
			tmpBoxProjectionMin = dotProduct(box->getPoint(7, boxTransform), tmpNormal);
			tmpBoxProjectionMax = dotProduct(box->getPoint(0, boxTransform), tmpNormal);
			if (tmpBoxProjectionMax < tmpBoxProjectionMin)
			{
				std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
			}
			computeProjection(sphere, sphereTransform, &tmpNormal, &tmpSphereProjectionMin, &tmpSphereProjectionMax);
			if (tmpBoxProjectionMax < tmpSphereProjectionMin ||
				tmpSphereProjectionMax < tmpBoxProjectionMin)
			{
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		// Check box's local X-axis
		{
			tmpTriangle = box->getTriangle(2, boxTransform);
			tmpNormal = tmpTriangle.computeNormal();
			tmpBoxProjectionMin = dotProduct(box->getPoint(0, boxTransform), tmpNormal);
			tmpBoxProjectionMax = dotProduct(box->getPoint(3, boxTransform), tmpNormal);
			if (tmpBoxProjectionMax < tmpBoxProjectionMin)
			{
				std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
			}
			computeProjection(sphere, sphereTransform, &tmpNormal, &tmpSphereProjectionMin, &tmpSphereProjectionMax);
			if (tmpBoxProjectionMax < tmpSphereProjectionMin ||
				tmpSphereProjectionMax < tmpBoxProjectionMin)
			{
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		// Check box's local Y-axis
		{
			tmpTriangle = box->getTriangle(4, boxTransform);
			tmpNormal = tmpTriangle.computeNormal();
			tmpBoxProjectionMin = dotProduct(box->getPoint(0, boxTransform), tmpNormal);
			tmpBoxProjectionMax = dotProduct(box->getPoint(1, boxTransform), tmpNormal);
			if (tmpBoxProjectionMax < tmpBoxProjectionMin)
			{
				std::swap(tmpBoxProjectionMin, tmpBoxProjectionMax);
			}
			computeProjection(sphere, sphereTransform, &tmpNormal, &tmpSphereProjectionMin, &tmpSphereProjectionMax);
			if (tmpBoxProjectionMax < tmpSphereProjectionMin ||
				tmpSphereProjectionMax < tmpBoxProjectionMin)
			{
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		// Check sphere vs box
		{
			tmpNormal = box->position - sphere->position;
			tmpNormal.normalize();
			tmpSphereProjectionMin = -sphere->radius;
			tmpSphereProjectionMax = sphere->radius;
			computeProjection(box, boxTransform, &tmpNormal, &tmpBoxProjectionMin, &tmpBoxProjectionMax);
			if (tmpBoxProjectionMax < tmpSphereProjectionMin ||
				tmpSphereProjectionMax < tmpBoxProjectionMin)
			{
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		if (computeCollisionInfo)
		{
			// TODO: compute collision info
		}
		return true;
	}

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform,
		bool computeCollisionInfo,
		CollisionInfo* collisionInfo)
	{
		Vector3D tmpNormal;
		float tmpBoxProjectionMin = 0.0f, tmpBoxProjectionMax = 0.0f;
		float tmpCapsuleProjectionMin = 0.0f, tmpCapsuleProjectionMax = 0.0f;

		Vector3D capsulePosition1 = capsule->position;
		capsulePosition1.j -= capsule->halfHeight;
		// TODO: transform capsulePosition1 via capsuleTransform
		Vector3D capsulePosition2 = capsule->position;
		capsulePosition2.j += capsule->halfHeight;
		// TODO: transform capsulePosition2 via capsuleTransform

		uint32_t  boxPointsCount = box->getPointsCount();
		for (uint32_t i = 0; i < boxPointsCount; ++i)
		{
			Vector3D boxPoint = box->getPoint(i, boxTransform);

			Vector3D closestCapsuleSpherePosition =
				computeClosestPointOnTheLine(capsulePosition1, capsulePosition2, boxPoint);

			tmpNormal = boxPoint - closestCapsuleSpherePosition;
			tmpNormal.normalize();
			tmpCapsuleProjectionMin = -capsule->radius;
			tmpCapsuleProjectionMax = capsule->radius;
			computeProjection(box, boxTransform, &tmpNormal, &tmpBoxProjectionMin, &tmpBoxProjectionMax);
			if (tmpBoxProjectionMax < tmpCapsuleProjectionMin ||
				tmpCapsuleProjectionMax < tmpBoxProjectionMin)
			{
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		if (computeCollisionInfo)
		{
			// TODO: compute collision info
		}
		return true;
	}

	bool checkCollision(
		BoxGeometry* box, 
		GeometryTransform* boxTransform, 
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		bool computeCollisionInfo,
		CollisionInfo* collisionInfo)
	{
		Point3D collisionPoint;

		switch (otherGeometry->type)
		{
		case GeometryPrimitiveType::TRIANGLE:
			break;

		case GeometryPrimitiveType::BOX:
		{
			BoxGeometry* otherBox = (BoxGeometry*)otherGeometry;
			Point3D tmpCollisionPoint;
			return checkCollision(box, boxTransform, otherBox, otherGeometryTransform, computeCollisionInfo, collisionInfo);
			return true;
		}

		case GeometryPrimitiveType::SPHERE:
		{
			SphereGeometry* otherSphere = (SphereGeometry*)otherGeometry;
			Point3D tmpCollisionPoint;
			return checkCollision(box, boxTransform, otherSphere, otherGeometryTransform, computeCollisionInfo, collisionInfo);
		}

		case GeometryPrimitiveType::CAPSULE:
			break;
		case GeometryPrimitiveType::TRUNCATED_PYRAMID:
			break;
		case GeometryPrimitiveType::RAY:
			break;
		case GeometryPrimitiveType::MESH:
			break;
		default:
			break;
		}
		return false;
	}

	bool checkCollision(
		SphereGeometry* sphere1, 
		GeometryTransform* sphere1Transform,
		SphereGeometry* sphere2,
		GeometryTransform* sphere2Transform,
		bool computeCollisionInfo,
		CollisionInfo* collisionInfo)
	{
		float maxR = sphere1->radius + sphere2->radius;

		// TODO: transform sphere1 & sphere2

		if (std::abs(sphere1->position.i - sphere2->position.i) > maxR)
		{
			return false;
		}
		if (std::abs(sphere1->position.j - sphere2->position.j) > maxR)
		{
			return false;
		}
		if (std::abs(sphere1->position.k - sphere2->position.k) > maxR)
		{
			return false;
		}

		return true;
	}
}
