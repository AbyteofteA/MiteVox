#ifndef DECIMATEMESH_H
#define DECIMATEMESH_H

#include "engine/Math/src/Vector.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"

#include <cstdint>

namespace mathem
{
	/// <summary>
	/// Decimates a triangle mesh by smallest edge removal
	/// </summary>
	/// <param name="meshPrimitive"></param>
	/// <param name="maxTrianglesCount"></param>
	void decimateMesh(mitevox::MeshPrimitive* meshPrimitive, size_t maxTrianglesCount);
}

#endif