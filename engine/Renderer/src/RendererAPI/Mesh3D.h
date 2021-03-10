
#ifndef RENDERER_MESH3D_H
#define RENDERER_MESH3D_H

#include <cstdlib>
#include <string>

namespace render
{
	struct Mesh3D
	{
		char type = 0;

		char isUploaded = 0;

		mathem::Point3D* v = nullptr;
		size_t amOfVertices = 0;
		unsigned int vertexBufferID = -1;

		mathem::Point3D vMax, vMin;

		mathem::Vector3D* vn = nullptr;
		size_t amOfNormals = 0;

		mathem::Point3D* vt = nullptr;
		size_t amOfTextureCoords = 0;

		Polygon* f = nullptr;
		size_t amOfFaces = 0;
	};
}

#endif