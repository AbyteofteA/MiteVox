#ifndef MAKECONVEXHULLFROMMESH_H
#define MAKECONVEXHULLFROMMESH_H

#include "engine/Math/src/Vector.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"

namespace mathem
{
	void makeConvexHullFromMeshBruteForce(mitevox::Mesh* mesh, mitevox::MeshPrimitive* convexHull);
}

#endif

