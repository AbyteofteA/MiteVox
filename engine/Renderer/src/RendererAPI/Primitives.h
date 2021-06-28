
#ifndef RENDERER_PRIMITIVES_H
#define RENDERER_PRIMITIVES_H


namespace render
{
	struct Point
	{
		mathem::Point3D position;
		//mathem::Vector3D normal;
		//mathem::Point3D textureCoord;
		render::ColorRGBAf color;
	};

	struct Polygon
	{
		long* positions = nullptr;
		long* textureCoords = nullptr;
		long* normals = nullptr;
		ColorRGBA* colors = nullptr;
		size_t amOfVertices = 0;
	};
}


#endif