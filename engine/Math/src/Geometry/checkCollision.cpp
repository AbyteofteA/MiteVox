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

		TriangleGeometry tmpTriangle = box1->getTrianglePositions(0);
		// TODO: transform tmpTriangle via box1Transform
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBox1ProjectionMin = 0.0f, tmpBox1ProjectionMax = 0.0f;
		float tmpBox2ProjectionMin = 0.0f, tmpBox2ProjectionMax = 0.0f;

		// Check box1's local Z-axis
		{
			tmpPositionMin = box1->getVertexPosition(7);
			tmpPositionMax = box1->getVertexPosition(0);
			// TODO: transform tmpPositionMin via box1Transform
			// TODO: transform tmpPositionMax via box1Transform
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
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		// Check box1's local X-axis
		{
			tmpTriangle = box1->getTrianglePositions(2);
			// TODO: transform tmpTriangle via box1Transform
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMin = box1->getVertexPosition(0);
			tmpPositionMax = box1->getVertexPosition(3);
			// TODO: transform tmpPositionMin via box1Transform
			// TODO: transform tmpPositionMax via box1Transform
			tmpBox1ProjectionMin = tmpPositionMin * tmpNormal;
			tmpBox1ProjectionMax = tmpPositionMax * tmpNormal;

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
			tmpTriangle = box1->getTrianglePositions(4);
			// TODO: transform tmpTriangle via box1Transform
			tmpNormal = tmpTriangle.computeNormal();
			
			tmpPositionMin = box1->getVertexPosition(0);
			tmpPositionMax = box1->getVertexPosition(1);
			// TODO: transform tmpPositionMin via box1Transform
			// TODO: transform tmpPositionMax via box1Transform
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
		TriangleGeometry tmpTriangle = box->getTrianglePositions(0);
		// TODO: transform tmpTriangle via boxTransform
		Vector3D tmpPositionMax;
		Vector3D tmpPositionMin;
		Vector3D tmpNormal = tmpTriangle.computeNormal();
		float tmpBoxProjectionMin = 0.0f, tmpBoxProjectionMax = 0.0f;
		float tmpSphereProjectionMin = 0.0f, tmpSphereProjectionMax = 0.0f;

		// Check box's local Z-axis
		{
			tmpPositionMin = box->getVertexPosition(7);
			tmpPositionMax = box->getVertexPosition(0);
			// TODO: transform tmpPositionMin via boxTransform
			// TODO: transform tmpPositionMax via boxTransform
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
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		// Check box's local X-axis
		{
			tmpTriangle = box->getTrianglePositions(2);
			// TODO: transform tmpTriangle via boxTransform
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMin = box->getVertexPosition(0);
			tmpPositionMax = box->getVertexPosition(3);
			// TODO: transform tmpPositionMin via boxTransform
			// TODO: transform tmpPositionMax via boxTransform
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
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		// Check box's local Y-axis
		{
			tmpTriangle = box->getTrianglePositions(4);
			// TODO: transform tmpTriangle via boxTransform
			tmpNormal = tmpTriangle.computeNormal();

			tmpPositionMin = box->getVertexPosition(0);
			tmpPositionMax = box->getVertexPosition(1);
			// TODO: transform tmpPositionMin via boxTransform
			// TODO: transform tmpPositionMax via boxTransform
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
				collisionInfo->type = CollisionType::NONE;
				return false;
			}
		}

		// Check sphere vs box
		{
			tmpNormal = box->translation - sphere->translation;
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

		Vector3D capsulePosition1 = capsule->translation;
		capsulePosition1.y() -= capsule->halfHeight;
		// TODO: transform capsulePosition1 via capsuleTransform
		Vector3D capsulePosition2 = capsule->translation;
		capsulePosition2.y() += capsule->halfHeight;
		// TODO: transform capsulePosition2 via capsuleTransform

		uint32_t  boxPointsCount = box->getVertecesCount();
		for (uint32_t i = 0; i < boxPointsCount; ++i)
		{
			Vector3D boxPoint = box->getVertexPosition(i);
			// TODO: transform boxPoint via boxTransform

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

		if (std::abs(sphere1->translation.x() - sphere2->translation.x()) > maxR)
		{
			return false;
		}
		if (std::abs(sphere1->translation.y() - sphere2->translation.y()) > maxR)
		{
			return false;
		}
		if (std::abs(sphere1->translation.z() - sphere2->translation.z()) > maxR)
		{
			return false;
		}

		return true;
	}
}
