/// checkCollision.h
/// 
/// Contains implementation of collision detection via the Hyperplane Separation Theorem

#ifndef CHECKCOLLISION_H
#define CHECKCOLLISION_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/SphereGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/CapsuleGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TruncatedPyramidGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/RayGeometry.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/Math/src/Geometry/CollisionInfo.h"
#include "engine/Math/src/Point3D.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	// TODO: checkCollision TRIANGLE vs TRIANGLE
	// TODO: checkCollision TRIANGLE vs BOX
	// TODO: checkCollision TRIANGLE vs SPHERE
	// TODO: checkCollision TRIANGLE vs CAPSULE
	// TODO: checkCollision TRIANGLE vs TRUNCATED_PYRAMID
	// TODO: checkCollision TRIANGLE vs RAY
	// TODO: checkCollision TRIANGLE vs MESH
	// TODO: checkCollision TRIANGLE vs GeometryPrimitiveBase

	/// checkCollision - BOX vs BOX
	/// 
	/// Algorithm:
	/// 1. Get (transformed) triangles 0, 2, 4 from box1
	/// 2. Extract normals from the triangles
	/// 3. Compute the box1's & box2's projections onto the normals (via dot product)
	/// 4. Check if the projections overlap
	bool checkCollision(
		BoxGeometry* box1,
		GeometryTransform* box1Transform,
		BoxGeometry* box2,
		GeometryTransform* box2Transform,
		bool computeCollisionInfo,
		CollisionInfo* collisionInfo = nullptr,
		bool isFirstPass = true);

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform,
		bool computeCollisionInfo,
		CollisionInfo* collisionInfo = nullptr);

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform,
		bool computeCollisionInfo,
		CollisionInfo* collisionInfo = nullptr);

	// TODO: checkCollision BOX vs TRUNCATED_PYRAMID
	// TODO: checkCollision BOX vs RAY
	// TODO: checkCollision BOX vs MESH

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		bool computeCollisionInfo,
		CollisionInfo* collisionInfo = nullptr);

	bool checkCollision(
		SphereGeometry* sphere1,
		GeometryTransform* sphere1Transform,
		SphereGeometry* sphere2,
		GeometryTransform* sphere2Transform,
		bool computeCollisionInfo,
		CollisionInfo* collisionInfo = nullptr);

	// TODO: checkCollision SPHERE vs CAPSULE
	// TODO: checkCollision SPHERE vs TRUNCATED_PYRAMID
	// TODO: checkCollision SPHERE vs RAY
	// TODO: checkCollision SPHERE vs MESH
	// TODO: checkCollision SPHERE vs GeometryPrimitiveBase

	// TODO: checkCollision CAPSULE vs CAPSULE
	// TODO: checkCollision CAPSULE vs TRUNCATED_PYRAMID
	// TODO: checkCollision CAPSULE vs RAY
	// TODO: checkCollision CAPSULE vs MESH
	// TODO: checkCollision CAPSULE vs GeometryPrimitiveBase

	// TODO: checkCollision TRUNCATED_PYRAMID vs TRUNCATED_PYRAMID
	// TODO: checkCollision TRUNCATED_PYRAMID vs RAY
	// TODO: checkCollision TRUNCATED_PYRAMID vs MESH
	// TODO: checkCollision TRUNCATED_PYRAMID vs GeometryPrimitiveBase

	// TODO: checkCollision RAY vs RAY
	// TODO: checkCollision RAY vs MESH
	// TODO: checkCollision RAY vs GeometryPrimitiveBase

	// TODO: checkCollision MESH vs MESH
	// TODO: checkCollision MESH vs GeometryPrimitiveBase

	// TODO: checkCollision GeometryPrimitiveBase vs GeometryPrimitiveBase

	// TODO: checkCollision ComplexGeometry vs ComplexGeometry
}

#endif