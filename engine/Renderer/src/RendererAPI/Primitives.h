
#ifndef RENDERER_PRIMITIVES_H
#define RENDERER_PRIMITIVES_H


namespace render
{
	struct Point
	{
		mathem::Point3D pos;
		render::ColorRGBAf color;
	};

	struct Polygon
	{
		long* p = nullptr;
		long* t = nullptr;
		long* n = nullptr;
		ColorRGBA* color = nullptr;
		size_t amOfVertices = 0;
	};
}


#endif