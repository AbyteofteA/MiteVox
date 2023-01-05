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
		CollisionInfo* collisionInfo = nullptr,
		bool isFirstPass = true);

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		AxisAlignedBoxGeometry* axisAlignedBox,
		CollisionInfo* collisionInfo = nullptr,
		bool isFirstPass = true);

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform,
		CollisionInfo* collisionInfo = nullptr);

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform,
		CollisionInfo* collisionInfo = nullptr);

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		TruncatedPyramidGeometry* truncatedPyramid,
		GeometryTransform* truncatedPyramidTransform,
		CollisionInfo* collisionInfo = nullptr);

	// TODO: checkCollision BOX vs MESH

	bool checkCollision(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionInfo* collisionInfo = nullptr);

	bool checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox1,
		AxisAlignedBoxGeometry* axisAlignedBox2,
		CollisionInfo* collisionInfo = nullptr);

	// TODO: checkCollision AXIS_ALIGNED_BOX vs SPHERE
	// TODO: checkCollision AXIS_ALIGNED_BOX vs CAPSULE
	// TODO: checkCollision AXIS_ALIGNED_BOX vs TRUNCATED_PYRAMID
	// TODO: checkCollision AXIS_ALIGNED_BOX vs MESH

	bool checkCollision(
		AxisAlignedBoxGeometry* axisAlignedBox,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionInfo* collisionInfo = nullptr);

	bool checkCollision(
		SphereGeometry* sphere1,
		GeometryTransform* sphere1Transform,
		SphereGeometry* sphere2,
		GeometryTransform* sphere2Transform,
		CollisionInfo* collisionInfo = nullptr);

	// TODO: checkCollision SPHERE vs CAPSULE
	// TODO: checkCollision SPHERE vs TRUNCATED_PYRAMID
	// TODO: checkCollision SPHERE vs MESH
	// TODO: checkCollision SPHERE vs GeometryPrimitiveBase

	// TODO: checkCollision CAPSULE vs CAPSULE
	// TODO: checkCollision CAPSULE vs TRUNCATED_PYRAMID
	// TODO: checkCollision CAPSULE vs MESH
	// TODO: checkCollision CAPSULE vs GeometryPrimitiveBase

	// TODO: checkCollision TRUNCATED_PYRAMID vs TRUNCATED_PYRAMID
	// TODO: checkCollision TRUNCATED_PYRAMID vs MESH
	// TODO: checkCollision TRUNCATED_PYRAMID vs GeometryPrimitiveBase

	// TODO: checkCollision MESH vs MESH
	// TODO: checkCollision MESH vs GeometryPrimitiveBase

	// TODO: checkCollision GeometryPrimitiveBase vs GeometryPrimitiveBase

	// TODO: checkCollision ComplexGeometry vs ComplexGeometry

	CollisionType checkCollision(
		ComplexGeometry* complexGeometry,
		GeometryTransform* complexGeometryTransform,
		AxisAlignedBoxGeometry* axisAlignedBox);
}

#endif