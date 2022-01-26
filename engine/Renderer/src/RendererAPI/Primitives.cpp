#include "Primitives.h"

#include <cstdlib>

namespace render
{
	void Polygon::setType(PolygonType polygonType)
	{
		type = polygonType;
	}

	void Polygon::setVertexSimple(size_t index, long positionIndex)
	{
		if (type != PolygonType::POSITIONS)
		{
			return;
		}
		positions[index] = positionIndex;
	}

	void Polygon::setVertexWithNormal(size_t index, long positionIndex, long normalIndex)
	{
		if (type != PolygonType::POSITIONS_AND_NORMALS)
		{
			return;
		}
		positions[index] = positionIndex;
		normals[index] = normalIndex;
	}

	void Polygon::setVertexWithTexture(size_t index, long positionIndex, long textureCoordIndex)
	{
		if (type != PolygonType::POSITIONS_AND_TEXTURE)
		{
			return;
		}
		positions[index] = positionIndex;
		textureCoords[index] = textureCoordIndex;
	}

	void Polygon::setVertexComplete(size_t index, long positionIndex, long normalIndex, long textureCoordIndex)
	{
		if (type != PolygonType::COMPLETE)
		{
			return;
		}
		positions[index] = positionIndex;
		normals[index] = normalIndex;
		textureCoords[index] = textureCoordIndex;
	}
}