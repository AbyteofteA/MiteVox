
#ifndef COMPUTEPROJECTION_H
#define COMPUTEPROJECTION_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/SphereGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/CapsuleGeometry.h"
// TODO: #include "engine/Math/src/Geometry/GeometryPrimitives/TruncatedPyramidGeometry.h"
// TODO: #include "engine/Math/src/Geometry/GeometryPrimitives/RayGeometry.h"
// TODO: #include "engine/MiteVox/src/Mesh/Mesh.h"

namespace mathem
{
	void computeProjection(
		BoxGeometry* box, 
		GeometryTransform* boxTransform, 
		Vector3D* vector, 
		float* min, 
		float* max);

	void computeProjection(
		SphereGeometry* sphere,
		GeometryTransform* sphereTransform,
		Vector3D* vector,
		float* min,
		float* max);

	void computeProjection(
		CapsuleGeometry* capsule,
		GeometryTransform* capsuleTransform,
		Vector3D* vector,
		float* min,
		float* max);
}

#endif
