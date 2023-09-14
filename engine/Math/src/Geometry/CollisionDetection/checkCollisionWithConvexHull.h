#ifndef CHECKCOLLISIONWITHCONVEXHULL_H
#define CHECKCOLLISIONWITHCONVEXHULL_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"

namespace mathem
{
	CollisionType checkCollisionConvexHullVsBox(
		GeometryPrimitiveBase* convexHull,
		GeometryTransform* convexHullTransform,
		GeometryPrimitiveBase* boxGeometry,
		GeometryTransform* boxTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionConvexHullVsAABB(
		GeometryPrimitiveBase* convexHull,
		GeometryTransform* convexHullTransform,
		GeometryPrimitiveBase* axisAlignedBox,
		GeometryTransform* axisAlignedBoxTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionConvexHullVsTruncatedPyramid(
		GeometryPrimitiveBase* convexHull,
		GeometryTransform* convexHullTransform,
		GeometryPrimitiveBase* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionProperties* collisionProperties);

	CollisionType checkCollisionConvexHullVsConvexHull(
		GeometryPrimitiveBase* convexHull1,
		GeometryTransform* convexHullTransform1,
		GeometryPrimitiveBase* convexHull2,
		GeometryTransform* convexHullTransform2,
		CollisionProperties* collisionProperties);
}

#endif