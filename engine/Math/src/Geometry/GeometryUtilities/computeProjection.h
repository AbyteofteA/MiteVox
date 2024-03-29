#ifndef COMPUTEPROJECTION_H
#define COMPUTEPROJECTION_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/SphereGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/CapsuleGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"

namespace mathem
{
	void computeProjectionForMesh(
		mitevox::Mesh* mesh,
		Vector3D* vector,
		float* min,
		float* max);

	void computeProjectionForMesh(
		mitevox::MeshPrimitive* meshPrimitive,
		Vector3D* vector,
		float* min,
		float* max);

	/// <summary>
	/// computeProjection for BoxGeometry, TruncatedPyramidGeometry and Mesh
	/// </summary>
	/// <param name="triangularMesh"></param>
	/// <param name="parentTransform"></param>
	/// <param name="vector"></param>
	/// <param name="min"></param>
	/// <param name="max"></param>
	void computeProjection(
		GeometryPrimitiveBase* triangularMesh,
		GeometryTransform* parentTransform,
		Vector3D* vector, 
		float* min, 
		float* max);

	void computeProjection(
		SphereGeometry* sphere,
		GeometryTransform* parentTransform,
		Vector3D* vector,
		float* min,
		float* max);

	void computeProjection(
		CapsuleGeometry* capsule,
		GeometryTransform* parentTransform,
		Vector3D* vector,
		float* min,
		float* max);

	void computeProjection(
		TriangleGeometry3D* triangle,
		GeometryTransform* parentTransform,
		Vector3D* vector,
		float* min,
		float* max);
}

#endif
