
#ifndef COMPUTEPROJECTION_H
#define COMPUTEPROJECTION_H

#include "engine/Math/src/LinearAlgebra/Vector3D.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/BoxGeometry.h"
#include "engine/Math/src/Geometry/SphereGeometry.h"
#include "engine/Math/src/Geometry/CapsuleGeometry.h"
// TODO: #include "engine/Math/src/Geometry/TruncatedPyramidGeometry.h"
// TODO: #include "engine/Math/src/Geometry/RayGeometry.h"
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
