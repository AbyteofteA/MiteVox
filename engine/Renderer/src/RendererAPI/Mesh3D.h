
#ifndef RENDERER_MESH3D_H
#define RENDERER_MESH3D_H

#include "engine/Math/src/Vector.h"
#include <cstdlib>
#include <string>

namespace render
{
	struct Mesh3D
	{
		char type = 0;

		char isUploaded = 0;

		mathem::Point3D* positions = nullptr;
		size_t positionsCount = 0;
		unsigned int vertexBufferID = -1;

		mathem::Point3D minPosition, maxPosition;

		mathem::Vector3D* normals = nullptr;
		size_t normalsCount = 0;

		mathem::Point3D* textureCoords = nullptr;
		size_t textureCoordsCount = 0;

		Polygon* polygons = nullptr;
		size_t polygonsCount = 0;
	};
}

#endif