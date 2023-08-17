#ifndef RENDERER_PRIMITIVES_H
#define RENDERER_PRIMITIVES_H

#include "engine/Math/src/Vector.h"
#include "engine/Renderer/src/RendererAPI/Color.h"

namespace render
{
	struct Point
	{
		mathem::Vector3D position;
		//mathem::Vector3D normal;
		//mathem::Vector3D textureCoord;
		render::ColorRGBAf color;
	};

	enum struct PolygonType
	{
		INVALID = -1,
		POSITIONS,
		POSITIONS_AND_NORMALS,
		POSITIONS_AND_TEXTURE,
		COMPLETE,
	};

	struct Polygon
	{
		PolygonType type = PolygonType::INVALID;
		long positions[3] = { 0 };
		long textureCoords[3] = { 0 };
		long normals[3] = { 0 };
		ColorRGBA colors[3];

		void setType(PolygonType polygonType);
		void setVertexSimple(size_t index, long positionIndex);
		void setVertexWithNormal(size_t index, long positionIndex, long normalIndex);
		void setVertexWithTexture(size_t index, long positionIndex, long textureCoordIndex);
		void setVertexComplete(size_t index, long positionIndex, long normalIndex, long textureCoordIndex);
	};
}


#endif