#include "MeshPrimitive.h"

namespace mitevox
{
	mitevox::BufferViewAccessor* MeshPrimitive::getPositions()
	{
		return attributes.positionAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getNormals()
	{
		return attributes.normalAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTangents()
	{
		return attributes.tangentAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTextureCoords_0()
	{
		return attributes.textureCoordAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTextureCoords_1()
	{
		return attributes.textureCoordAccessor_1;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getColors_0()
	{
		return attributes.colorAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getJoints_0()
	{
		return attributes.jointsAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getWeights_0()
	{
		return attributes.weightsAccessor_0;
	}


	mitevox::BufferViewAccessor* MeshPrimitive::getIndeces()
	{
		return indecesAccessor;
	}
}