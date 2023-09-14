#ifndef MAKETETRAHEDRON_H
#define MAKETETRAHEDRON_H

#include "engine/Math/src/Vector.h"
#include "engine/MiteVox/src/Mesh/MeshPrimitive.h"

namespace mathem
{
	bool makeTetrahedron(
		Vector3D point1,
		Vector3D point2,
		Vector3D point3,
		Vector3D point4,
		mitevox::MeshPrimitive* resultMeshPrimitive);
}

#endif
