/// checkCollision.h
/// 
/// Contains implementation of collision detection via the Hyperplane Separation Theorem

#ifndef CHECKCOLLISION_H
#define CHECKCOLLISION_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionTable.h"
#include "engine/Math/src/Vector.h"

#include <cassert>
#undef NDEBUG

namespace mathem
{
	template <class T>
	inline CollisionType checkCollision(T object1, T object2, CollisionInfo<T>* collisionInfo, float equalityTolerance)
	{
		collisionInfo->object1 = object1;
		collisionInfo->object2 = object2;
		collisionInfo->properties.type = CollisionType::NONE;

		if (object1->isSleeping() &&
			object2->isSleeping())
		{
			return CollisionType::NONE;
		}

		ComplexGeometry* complexGeometry1 = object1->getCollider();
		ComplexGeometry* complexGeometry2 = object2->getCollider();
		if (complexGeometry1->getType() == mathem::GeometryType::NONE ||
			complexGeometry2->getType() == mathem::GeometryType::NONE)
		{
			collisionInfo->reset();
			return CollisionType::NONE;
		}

		GeometryTransform* complexGeometryTransform1 = object1->getResultTransform();
		GeometryTransform* complexGeometryTransform2 = object2->getResultTransform();

		size_t objectPrimitivesCount1 = complexGeometry1->getPrimitivesCount();
		for (size_t primitiveIndex1 = 0; primitiveIndex1 < objectPrimitivesCount1; primitiveIndex1++)
		{
			GeometryPrimitiveBase* geometryPrimitive1 = complexGeometry1->getPrimitive(primitiveIndex1);
			size_t objectPrimitivesCount2 = complexGeometry2->getPrimitivesCount();
			for (size_t primitiveIndex2 = 0; primitiveIndex2 < objectPrimitivesCount2; primitiveIndex2++)
			{
				GeometryPrimitiveBase* geometryPrimitive2 = complexGeometry2->getPrimitive(primitiveIndex2);
				CollisionProperties collisionProperties;
				CollisionTable::checkCollision(
					geometryPrimitive1, complexGeometryTransform1, geometryPrimitive2, complexGeometryTransform2, &collisionProperties);
				collisionInfo->properties.concatenate(collisionProperties, equalityTolerance);
			}
		}

		if (collisionInfo->properties.type != CollisionType::NONE)
		{
			// Check the order of objects
			if (collisionInfo->properties.normalBelongsToTheFirst == false)
			{
				collisionInfo->object1 = object2;
				collisionInfo->object2 = object1;
				collisionInfo->properties.normalBelongsToTheFirst = true;
			}

			// Make sure that the normal looks towards the second geometry
			if (collisionInfo->properties.penetrationDepth < 0.0f)
			{
				collisionInfo->properties.penetrationDepth = -collisionInfo->properties.penetrationDepth;
				collisionInfo->properties.normal = -collisionInfo->properties.normal;
			}

			Vector3D distance = collisionInfo->object2->transform.getPosition() - collisionInfo->object1->transform.getPosition();
			//assert(("ERROR: Normal is not alligned", distance * collisionInfo->properties.normal >= 0.0f));
			assert(("ERROR: penetrationDepth is negative", collisionInfo->properties.penetrationDepth >= 0.0f));
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