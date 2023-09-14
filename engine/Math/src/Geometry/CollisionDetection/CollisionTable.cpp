#include "collisionTable.h"

#include "checkCollisionWithBox.h"
#include "checkCollisionWithAxisAlignedBox.h"
#include "checkCollisionWithSphere.h"
#include "checkCollisionWithPoint.h"
#include "checkCollisionWithConvexHull.h"

#include <cassert>

namespace mathem
{
	bool CollisionTable::needToInit = true;
	std::mutex CollisionTable::mutex;
	CollisionChecker CollisionTable::collisionTable[(size_t)GeometryPrimitiveType::count][(size_t)GeometryPrimitiveType::count];

	void CollisionTable::tryInit()
	{
		if (needToInit == false)
		{
			return;
		}

		std::lock_guard<std::mutex> lock(mutex);

		needToInit = false;

		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::SPHERE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::CAPSULE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::NONE][(size_t)GeometryPrimitiveType::CONVEX_HULL] = nullptr;

		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = checkCollisionPointVsAABB;
		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::BOX] = checkCollisionPointVsBox;
		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::SPHERE] = checkCollisionPointVsSphere;
		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::CAPSULE] = checkCollisionPointVsCapsule;
		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::POINT][(size_t)GeometryPrimitiveType::CONVEX_HULL] = nullptr;

		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::SPHERE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::CAPSULE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::LINE][(size_t)GeometryPrimitiveType::CONVEX_HULL] = nullptr;

		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::SPHERE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::CAPSULE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::RAY][(size_t)GeometryPrimitiveType::CONVEX_HULL] = nullptr;

		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::SPHERE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::CAPSULE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::PLANE][(size_t)GeometryPrimitiveType::CONVEX_HULL] = nullptr;

		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = checkCollisionAABBvsAABB;
		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::BOX] = checkCollisionAABBvsBox;
		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::SPHERE] = checkCollisionAABBvsSphere;
		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::CAPSULE] = checkCollisionAABBvsCapsule;
		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = checkCollisionAABBvsTruncatedPyramid;
		collisionTable[(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX][(size_t)GeometryPrimitiveType::CONVEX_HULL] = checkCollisionAABBvsConvexHull;

		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = checkCollisionBoxVsAABB;
		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::BOX] = checkCollisionBoxVsBox;
		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::SPHERE] = checkCollisionBoxVsSphere;
		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::CAPSULE] = checkCollisionBoxVsCapsule;
		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = checkCollisionBoxVsTruncatedPyramid;
		collisionTable[(size_t)GeometryPrimitiveType::BOX][(size_t)GeometryPrimitiveType::CONVEX_HULL] = checkCollisionBoxVsConvexHull;

		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = checkCollisionSphereVsAABB;
		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::BOX] = checkCollisionSphereVsBox;
		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::SPHERE] = checkCollisionSphereVsSphere;
		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::CAPSULE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::SPHERE][(size_t)GeometryPrimitiveType::CONVEX_HULL] = nullptr;

		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::SPHERE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::CAPSULE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CAPSULE][(size_t)GeometryPrimitiveType::CONVEX_HULL] = nullptr;

		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::BOX] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::SPHERE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::CAPSULE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID][(size_t)GeometryPrimitiveType::CONVEX_HULL] = nullptr;

		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::NONE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::POINT] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::LINE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::RAY] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::PLANE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::AXIS_ALIGNED_BOX] = checkCollisionConvexHullVsAABB;
		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::BOX] = checkCollisionConvexHullVsBox;
		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::SPHERE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::CAPSULE] = nullptr;
		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::TRUNCATED_PYRAMID] = checkCollisionConvexHullVsTruncatedPyramid;
		collisionTable[(size_t)GeometryPrimitiveType::CONVEX_HULL][(size_t)GeometryPrimitiveType::CONVEX_HULL] = checkCollisionConvexHullVsConvexHull;
	}

	CollisionType CollisionTable::checkCollision(
		GeometryPrimitiveBase* geometryPrimitive1,
		GeometryTransform* geometryPrimitive1Transform,
		GeometryPrimitiveBase* geometryPrimitive2,
		GeometryTransform* geometryPrimitive2Transform,
		CollisionProperties* collisionProperties)
	{
		GeometryPrimitiveType type1 = geometryPrimitive1->getType();
		GeometryPrimitiveType type2 = geometryPrimitive2->getType();
		CollisionChecker collisionChecker = collisionTable[(size_t)type1][(size_t)type2];
		assert(collisionChecker != nullptr);

		return collisionChecker(geometryPrimitive1, geometryPrimitive1Transform, geometryPrimitive2, geometryPrimitive2Transform, collisionProperties);
	}

	CollisionTable::CollisionTable()
	{

	}

	CollisionTable::~CollisionTable()
	{

	}
}