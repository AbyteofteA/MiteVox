/// checkCollision.h
/// 
/// Contains implementation of collision detection via the Hyperplane Separation Theorem

#ifndef CHECKCOLLISION_H
#define CHECKCOLLISION_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/SphereGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/CapsuleGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TruncatedPyramidGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/RayGeometry.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	CollisionType checkCollision(
		Vector3D* point,
		BoxGeometry* box,
		GeometryTransform* boxTransform);

	CollisionType checkCollision(
		Vector3D* point,
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform);

	CollisionType checkCollision(
		Vector3D* point,
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform);

	CollisionType checkCollision(
		Vector3D* point,
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform);

	// TODO: checkCollision POINT vs TRUNCATED_PYRAMID
	// TODO: checkCollision POINT vs MESH

	CollisionType checkCollision(
		Vector3D* point,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollision(
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties);

	// TODO: checkCollision CAPSULE vs CAPSULE
	// TODO: checkCollision CAPSULE vs TRUNCATED_PYRAMID
	// TODO: checkCollision CAPSULE vs MESH
	// TODO: checkCollision CAPSULE vs RAY
	// TODO: checkCollision CAPSULE vs GeometryPrimitiveBase

	// TODO: checkCollision TRUNCATED_PYRAMID vs TRUNCATED_PYRAMID
	// TODO: checkCollision TRUNCATED_PYRAMID vs MESH
	// TODO: checkCollision TRUNCATED_PYRAMID vs RAY
	// TODO: checkCollision TRUNCATED_PYRAMID vs GeometryPrimitiveBase

	// TODO: checkCollision MESH vs MESH
	// TODO: checkCollision MESH vs RAY
	// TODO: checkCollision MESH vs GeometryPrimitiveBase

	CollisionType checkCollision(
		GeometryPrimitiveBase* geometryPrimitive1,
		GeometryTransform* geometryPrimitive1Transform,
		GeometryPrimitiveBase* geometryPrimitive2,
		GeometryTransform* geometryPrimitive2Transform,
		CollisionProperties* collisionProperties,
		float equalityTolerance);

	template <class T>
	inline CollisionType checkCollision(T object1, T object2, CollisionInfo<T>* collisionInfo, float equalityTolerance)
	{
		collisionInfo->object1 = object1;
		collisionInfo->object2 = object2;
		collisionInfo->properties.type = CollisionType::NONE;

		ComplexGeometry* complexGeometry1 = object1->getCollider();
		GeometryTransform* complexGeometryTransform1 = object1->getTransform();
		ComplexGeometry* complexGeometry2 = object2->getCollider();
		GeometryTransform* complexGeometryTransform2 = object2->getTransform();

		size_t objectPrimitivesCount1 = complexGeometry1->primitives.getElementsCount();
		for (size_t primitiveIndex1 = 0; primitiveIndex1 < objectPrimitivesCount1; primitiveIndex1++)
		{
			GeometryPrimitiveBase* geometryPrimitive1 = complexGeometry1->primitives.getElement(primitiveIndex1);
			size_t objectPrimitivesCount2 = complexGeometry2->primitives.getElementsCount();
			for (size_t primitiveIndex2 = 0; primitiveIndex2 < objectPrimitivesCount2; primitiveIndex2++)
			{
				GeometryPrimitiveBase* geometryPrimitive2 = complexGeometry2->primitives.getElement(primitiveIndex2);
				CollisionProperties collisionProperties;
				checkCollision(
					geometryPrimitive1, complexGeometryTransform1, geometryPrimitive2, complexGeometryTransform2, &collisionProperties, equalityTolerance);
				collisionInfo->properties.concatenate(collisionProperties, equalityTolerance);
			}
		}
		return collisionInfo->properties.type;
	}

	template <class T>
	inline void checkCollisions(std::vector<T>* objects, safety::SafeArray<CollisionInfo<T>>* collisions, float equalityTolerance)
	{
		size_t objectsCount = objects->size();
		for (size_t i = 0; i < objectsCount; i++)
		{
			for (size_t j = i + 1; j < objectsCount; j++)
			{
				CollisionInfo<T> collisionInfo;
				if (checkCollision<T>(objects->at(i), objects->at(j), &collisionInfo, equalityTolerance) != CollisionType::NONE)
				{
					collisions->appendElement(collisionInfo);
				}
			}
		}
	}

	template <class T>
	inline void checkCollisions(safety::SafeArray<T>* objects, safety::SafeArray<CollisionInfo<T>>* collisions, float equalityTolerance)
	{
		size_t objectsCount = objects->getElementsCount();
		for (size_t i = 0; i < objectsCount; i++)
		{
			for (size_t j = i + 1; j < objectsCount; j++)
			{
				CollisionInfo<T> collisionInfo;
				if (checkCollision<T>(objects->getElement(i), objects->getElement(j), &collisionInfo, equalityTolerance) != CollisionType::NONE)
				{
					collisions->appendElement(collisionInfo);
				}
			}
		}
	}
}

#endif