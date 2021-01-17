
#ifndef RENDERER_MESH3D_H
#define RENDERER_MESH3D_H

#include <cstdlib>
#include <string>

namespace render
{
	struct Polygon
	{
		long* p = nullptr;
		long* t = nullptr;
		long* n = nullptr;
		ColorRGBA* color = nullptr;
		int amOfVertices = 0;
	};

	struct Mesh3D
	{
		char type = 0;

		char isUploaded = 0;

		mathem::Point3D* v = nullptr;
		long amOfVertices = 0;
		unsigned int vertexBufferID = -1;

		mathem::Point3D vMax, vMin;

		mathem::Vector3D* vn = nullptr;
		long amOfNormals = 0;

		mathem::Point3D* vt = nullptr;
		long amOfTextureCoords = 0;

		Polygon* f = nullptr;
		long amOfFaces = 0;
	};
}

#endif