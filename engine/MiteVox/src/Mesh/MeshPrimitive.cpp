#include "MeshPrimitive.h"

namespace mitevox
{
	void MeshPrimitive::initTriangles(
		size_t count,
		bool normals,
		bool textureCoords_0,
		bool textureCoords_1,
		bool colors_0,
		bool joints_0,
		bool weights_0,
		bool tangents)
	{
		topologyType = TopologyType::TRIANGLES;
		material = new Material();

		attributes.byName.positionAccessor = new BufferViewAccessor();

		uint16_t byteStride = 12;

		if (normals)
		{
			attributes.byName.normalAccessor = new BufferViewAccessor();
			byteStride += 12;
		}
		if (textureCoords_0)
		{
			attributes.byName.textureCoordAccessor_0 = new BufferViewAccessor();
			byteStride += 8;
		}
		if (textureCoords_1)
		{
			attributes.byName.textureCoordAccessor_1 = new BufferViewAccessor();
			byteStride += 8;
		}
		if (colors_0)
		{
			attributes.byName.colorAccessor_0 = new BufferViewAccessor();
			byteStride += 16;
		}
		if (joints_0)
		{
			attributes.byName.jointsAccessor_0 = new BufferViewAccessor();
			// TODO: byteStride += ;
		}
		if (weights_0)
		{
			attributes.byName.weightsAccessor_0 = new BufferViewAccessor();
			// TODO: byteStride += ;
		}
		if (tangents)
		{
			attributes.byName.tangentAccessor = new BufferViewAccessor();
			// TODO: byteStride += ;
		}

		safety::SafeByteArray* meshPrimitiveBuffer = new safety::SafeByteArray();
		attributes.byName.positionAccessor->init(meshPrimitiveBuffer, byteStride, 0, ComponentDataType::FLOAT, BufferViewAccessor::Type::VEC3);
		uint64_t byteOffset = 12;
		if (normals)
		{
			attributes.byName.normalAccessor->init(meshPrimitiveBuffer, byteStride, byteOffset, ComponentDataType::FLOAT, BufferViewAccessor::Type::VEC3);
			byteOffset += 12;
		}
		if (textureCoords_0)
		{
			attributes.byName.textureCoordAccessor_0->init(meshPrimitiveBuffer, byteStride, byteOffset, ComponentDataType::FLOAT, BufferViewAccessor::Type::VEC2);
			byteOffset += 8;
		}
		if (textureCoords_1)
		{
			attributes.byName.textureCoordAccessor_1->init(meshPrimitiveBuffer, byteStride, byteOffset, ComponentDataType::FLOAT, BufferViewAccessor::Type::VEC2);
			byteOffset += 8;
		}
		if (colors_0)
		{
			attributes.byName.colorAccessor_0->init(meshPrimitiveBuffer, byteStride, byteOffset, ComponentDataType::FLOAT, BufferViewAccessor::Type::VEC4);
			byteOffset += 16;
		}
		if (joints_0)
		{
			// TODO: attributes.byName.jointsAccessor_0->init(meshPrimitiveBuffer, byteStride, byteOffset, ComponentDataType::FLOAT, BufferViewAccessor::Type::);
			// TODO: byteOffset += ;
		}
		if (weights_0)
		{
			// TODO: attributes.byName.weightsAccessor_0->init(meshPrimitiveBuffer, byteStride, byteOffset, ComponentDataType::FLOAT, BufferViewAccessor::Type::);
			// TODO: byteOffset += ;
		}
		if (tangents)
		{
			// TODO: attributes.byName.tangentAccessor->init(meshPrimitiveBuffer, byteStride, byteOffset, ComponentDataType::FLOAT, BufferViewAccessor::Type::);
			// TODO: byteOffset += ;
		}

		meshPrimitiveBuffer->reserve(count * byteStride);
	}

	void MeshPrimitive::makeCopyForAnimationTo(MeshPrimitive* resultMeshPrimitive)
	{
		attributes.makeSeparateCopyTo(&resultMeshPrimitive->attributes);
		if (indecesAccessor)
		{
			resultMeshPrimitive->indecesAccessor = new BufferViewAccessor();
			indecesAccessor->makeSeparateCopyTo(resultMeshPrimitive->indecesAccessor);
		}
		resultMeshPrimitive->material = material; // TODO: copy material
		resultMeshPrimitive->topologyType = topologyType;
		resultMeshPrimitive->ID = 0;

		/// NOTE: We don't copy morphTargets because resultMeshPrimitive will store 
		/// only the result of animation.
	}

	void MeshPrimitive::tryGenerateTangents()
	{
		return;

		if (attributes.byName.normalAccessor == nullptr ||
			attributes.byName.tangentAccessor != nullptr ||
			topologyType != TopologyType::TRIANGLES)
		{
			return;
		}

		attributes.byName.tangentAccessor = new BufferViewAccessor();
		attributes.byName.tangentAccessor->count = attributes.byName.normalAccessor->count;
		attributes.byName.tangentAccessor->componentType = attributes.byName.normalAccessor->componentType;
		attributes.byName.tangentAccessor->type = BufferViewAccessor::Type::VEC4;
		
		// TODO: tryGenerateTangents()
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getPositions()
	{
		return attributes.byName.positionAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getNormals()
	{
		return attributes.byName.normalAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTangents()
	{
		return attributes.byName.tangentAccessor;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTextureCoords_0()
	{
		return attributes.byName.textureCoordAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getTextureCoords_1()
	{
		return attributes.byName.textureCoordAccessor_1;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getColors_0()
	{
		return attributes.byName.colorAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getJoints_0()
	{
		return attributes.byName.jointsAccessor_0;
	}

	mitevox::BufferViewAccessor* MeshPrimitive::getWeights_0()
	{
		return attributes.byName.weightsAccessor_0;
	}


	mitevox::BufferViewAccessor* MeshPrimitive::getIndeces()
	{
		return indecesAccessor;
	}

	mathem::Vector3D MeshPrimitive::getMinPosition()
	{
		safety::SafeFloatArray minArray = getPositions()->min;
		mathem::Vector3D min(&minArray);
		return min;
	}

	mathem::Vector3D MeshPrimitive::getMaxPosition()
	{
		safety::SafeFloatArray maxArray = getPositions()->max;
		mathem::Vector3D max(&maxArray);
		return max;
	}
	
	void MeshPrimitive::reserve(size_t topologyElementsCount)
	{
		if (topologyElementsCount == 0)
		{
			return;
		}
		reserveVerteces(toVertecesCount(topologyElementsCount));
	}

	void MeshPrimitive::resize(size_t topologyElementsCount)
	{
		if (topologyElementsCount == 0)
		{
			return;
		}
		resizeVerteces(toVertecesCount(topologyElementsCount));
	}

	size_t MeshPrimitive::appendTopologyElements(size_t topologyElementsCount)
	{
		size_t vertecesCount = getVertecesCount();
		if (vertecesCount == 0)
		{
			resize(topologyElementsCount);
			return toVertecesCount(topologyElementsCount) - 1; // TODO: not optimal
		}

		switch (topologyType)
		{
		case mitevox::POINTS:
		case mitevox::LINE_LOOP:
		case mitevox::LINE_STRIP:
		case mitevox::TRIANGLE_STRIP:
		case mitevox::TRIANGLE_FAN:
			vertecesCount += topologyElementsCount;
			break;
		case mitevox::LINES:
			vertecesCount += 2 * topologyElementsCount;
			break;
		case mitevox::TRIANGLES:
			vertecesCount += 3 * topologyElementsCount;
			break;
		default:
			break;
		}

		resizeVerteces(vertecesCount);
		return vertecesCount - 1;
	}

	void MeshPrimitive::clear()
	{
		for (size_t i = 0; i < attributes.attributesCount; ++i)
		{
			attributes.byIndex[i]->clear();
		}
	}

	bool MeshPrimitive::isTriangularMesh()
	{
		switch (topologyType)
		{
		case mitevox::TRIANGLES:
		case mitevox::TRIANGLE_STRIP:
		case mitevox::TRIANGLE_FAN:
			return true;

		default:
			return false;
		}
	}

	uint32_t MeshPrimitive::getVertecesCount()
	{
		if (indecesAccessor != nullptr)
		{
			return getIndeces()->count;
		}
		else
		{
			return getPositions()->count;
		}
	}

	mathem::Vector3D MeshPrimitive::getVertexPosition(uint32_t index)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			return getPositions()->getVector3D(actualPointIndex);
		}
		else
		{
			return getPositions()->getVector3D(index);
		}
	}

	mathem::Vector3D MeshPrimitive::getVertexNormal(uint32_t index)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			return getNormals()->getVector3D(actualPointIndex);
		}
		else
		{
			return getNormals()->getVector3D(index);
		}
	}

	mathem::Vector2D MeshPrimitive::getVertexTextureCoords_0(uint32_t index)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			return getTextureCoords_0()->getVector2D(actualPointIndex);
		}
		else
		{
			return getTextureCoords_0()->getVector2D(index);
		}
	}

	mathem::Vector2D MeshPrimitive::getVertexTextureCoords_1(uint32_t index)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			return getTextureCoords_1()->getVector2D(actualPointIndex);
		}
		else
		{
			return getTextureCoords_1()->getVector2D(index);
		}
	}

	uint32_t MeshPrimitive::getTrianglesCount()
	{
		uint32_t pointsCount = getVertecesCount();

		switch (topologyType)
		{
		case mitevox::TRIANGLES:
			return pointsCount / 3;

		case mitevox::TRIANGLE_STRIP:
		case mitevox::TRIANGLE_FAN:
			return pointsCount - 2;

		default:
			return 0;
		}

		return 0;
	}

	void MeshPrimitive::getTriangleVertexIndeces(uint32_t triangleIndex, uint32_t* vertex1, uint32_t* vertex2, uint32_t* vertex3)
	{
		switch (topologyType)
		{
		case mitevox::TRIANGLES:
		{
			uint32_t firstPointIndex = triangleIndex * 3;
			*vertex1 = firstPointIndex;
			*vertex2 = firstPointIndex + 1;
			*vertex3 = firstPointIndex + 2;
			break;
		}
		case mitevox::TRIANGLE_STRIP:
		{
			*vertex1 = triangleIndex;
			if (triangleIndex & 1)
			{
				*vertex2 = triangleIndex + 2;
				*vertex3 = triangleIndex + 1;
			}
			else
			{
				*vertex2 = triangleIndex + 1;
				*vertex3 = triangleIndex + 2;
			}
			break;
		}
		case mitevox::TRIANGLE_FAN:
		{
			uint32_t secondPointIndex = triangleIndex * 2 + 1;
			*vertex1 = secondPointIndex;
			*vertex2 = secondPointIndex + 1;
			*vertex3 = 0;
			break;
		}
		default:
			break;
		}
	}

	mathem::TriangleGeometry3D MeshPrimitive::getTrianglePositions(uint32_t index)
	{
		mathem::TriangleGeometry3D resultTriangle;
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		resultTriangle.point1 = getVertexPosition(vertex1);
		resultTriangle.point2 = getVertexPosition(vertex2);
		resultTriangle.point3 = getVertexPosition(vertex3);
		return resultTriangle;
	}

	mathem::TriangleGeometry3D MeshPrimitive::getTriangleNormals(uint32_t index)
	{
		mathem::TriangleGeometry3D resultTriangle;
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		resultTriangle.point1 = getVertexNormal(vertex1);
		resultTriangle.point2 = getVertexNormal(vertex2);
		resultTriangle.point3 = getVertexNormal(vertex3);
		return resultTriangle;
	}

	mathem::TriangleGeometry2D MeshPrimitive::getTriangleTextureCoords_0(uint32_t index)
	{
		mathem::TriangleGeometry2D resultTriangle;
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		resultTriangle.point1 = getVertexTextureCoords_0(vertex1);
		resultTriangle.point2 = getVertexTextureCoords_0(vertex2);
		resultTriangle.point3 = getVertexTextureCoords_0(vertex3);
		return resultTriangle;
	}

	mathem::TriangleGeometry2D MeshPrimitive::getTriangleTextureCoords_1(uint32_t index)
	{
		mathem::TriangleGeometry2D resultTriangle;
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		resultTriangle.point1 = getVertexTextureCoords_1(vertex1);
		resultTriangle.point2 = getVertexTextureCoords_1(vertex2);
		resultTriangle.point3 = getVertexTextureCoords_1(vertex3);
		return resultTriangle;
	}

	void MeshPrimitive::setVertexPosition(uint32_t index, mathem::Vector3D position)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			getPositions()->setVector3D(actualPointIndex, position);
		}
		else
		{
			getPositions()->setVector3D(index, position);
		}
	}

	void MeshPrimitive::setVertexNormal(uint32_t index, mathem::Vector3D normal)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			getNormals()->setVector3D(actualPointIndex, normal);
		}
		else
		{
			getNormals()->setVector3D(index, normal);
		}
	}

	void MeshPrimitive::setTrianglePositions(uint32_t index, mathem::Vector3D position1, mathem::Vector3D position2, mathem::Vector3D position3)
	{
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		setVertexPosition(vertex1, position1);
		setVertexPosition(vertex2, position2);
		setVertexPosition(vertex3, position3);
	}

	void MeshPrimitive::setTriangleNormals(uint32_t index, mathem::Vector3D normal1, mathem::Vector3D normal2, mathem::Vector3D normal3)
	{
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		setVertexNormal(vertex1, normal1);
		setVertexNormal(vertex2, normal2);
		setVertexNormal(vertex3, normal3);
	}

	mathem::Vector3D MeshPrimitive::getMorphVertexPosition(uint32_t morphIndex, uint32_t index)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			return morphTargets.getElement(morphIndex)->byName.positionAccessor->getVector3D(actualPointIndex);
		}
		else
		{
			return morphTargets.getElement(morphIndex)->byName.positionAccessor->getVector3D(index);
		}
	}

	mathem::Vector3D MeshPrimitive::getMorphVertexNormal(uint32_t morphIndex, uint32_t index)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			return morphTargets.getElement(morphIndex)->byName.normalAccessor->getVector3D(actualPointIndex);
		}
		else
		{
			return morphTargets.getElement(morphIndex)->byName.normalAccessor->getVector3D(index);
		}
	}

	void MeshPrimitive::setMorphVertexPosition(uint32_t morphIndex, uint32_t index, mathem::Vector3D position)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			morphTargets.getElement(morphIndex)->byName.positionAccessor->setVector3D(actualPointIndex, position);
		}
		else
		{
			morphTargets.getElement(morphIndex)->byName.positionAccessor->setVector3D(index, position);
		}
	}

	void MeshPrimitive::setMorphVertexNormal(uint32_t morphIndex, uint32_t index, mathem::Vector3D normal)
	{
		if (indecesAccessor != nullptr)
		{
			uint32_t actualPointIndex = getIndeces()->getElementsComponentAsUint(index, 0);
			morphTargets.getElement(morphIndex)->byName.normalAccessor->setVector3D(actualPointIndex, normal);
		}
		else
		{
			morphTargets.getElement(morphIndex)->byName.normalAccessor->setVector3D(index, normal);
		}
	}

	size_t MeshPrimitive::toVertecesCount(size_t topologyElementsCount)
	{
		if (topologyElementsCount == 0)
		{
			return 0;
		}

		switch (topologyType)
		{
		case mitevox::POINTS:
			return topologyElementsCount;
			break;
		case mitevox::LINES:
			return topologyElementsCount * 2;
			break;
		case mitevox::LINE_LOOP:
		case mitevox::LINE_STRIP:
			return topologyElementsCount + 1;
			break;
		case mitevox::TRIANGLES:
			return topologyElementsCount * 3;
			break;
		case mitevox::TRIANGLE_STRIP:
		case mitevox::TRIANGLE_FAN:
			return topologyElementsCount + 2;
			break;
		default:
			break;
		}
		return 0;
	}

	void MeshPrimitive::reserveVerteces(size_t count)
	{
		for (size_t i = 0; i < attributes.attributesCount; ++i)
		{
			if (attributes.byIndex[i])
			{
				attributes.byIndex[i]->reserveElements(count);
			}
		}
	}

	void MeshPrimitive::resizeVerteces(size_t count)
	{
		for (size_t i = 0; i < attributes.attributesCount; ++i)
		{
			if (attributes.byIndex[i])
			{
				attributes.byIndex[i]->resizeElements(count);
			}
		}
	}
}