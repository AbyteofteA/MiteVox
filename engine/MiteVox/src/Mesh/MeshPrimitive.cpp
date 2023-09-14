#include "MeshPrimitive.h"

#include <algorithm>
#include <iostream>

namespace mitevox
{
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
	
	size_t MeshPrimitive::getTopologyElementsCount()
	{
		size_t vertecesCount = getVertecesOrIndecesCount();
		return toTopologyElementsCount(vertecesCount); // TODO: not optimal
	}

	void MeshPrimitive::reserveTopologyElements(size_t topologyElementsCount)
	{
		if (topologyElementsCount == 0)
		{
			return;
		}
		reserveVerteces(toVertecesCount(topologyElementsCount));
	}

	void MeshPrimitive::resizeTopologyElements(size_t topologyElementsCount)
	{
		if (topologyElementsCount == 0)
		{
			return;
		}
		resizeVerteces(toVertecesCount(topologyElementsCount));
	}

	size_t MeshPrimitive::appendTopologyElements(size_t topologyElementsCount)
	{
		if (topologyElementsCount == 0)
		{
			return 0;
		}
		size_t newTopologyElementsCount = getTopologyElementsCount() + topologyElementsCount;
		resizeTopologyElements(newTopologyElementsCount);
		return toVertecesCount(newTopologyElementsCount) - 1; // TODO: not optimal
	}

	void MeshPrimitive::removeLastTopologyElement()
	{
		resizeTopologyElements(getTopologyElementsCount() - 1);
	}

	void MeshPrimitive::clear()
	{
		for (size_t i = 0; i < attributes.attributesCount; ++i)
		{
			if (attributes.byIndex[i])
			{
				attributes.byIndex[i]->clear();
			}
		}
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

	void MeshPrimitive::tryGenerateFlatNormals(size_t firstIndex, size_t lastIndex)
	{
		if (attributes.byName.normalAccessor == nullptr)
		{
			return;
		}

		size_t trianglesCount = getTrianglesCount();
		firstIndex = std::min(firstIndex, trianglesCount - 1);
		lastIndex = std::min(lastIndex, trianglesCount - 1);

		for (size_t i = firstIndex; i <= lastIndex; ++i)
		{
			mathem::TriangleGeometry3D triangle = getTrianglePositions(i);
			mathem::Vector3D normal = triangle.computeNormal();
			setTriangleNormals(i, normal, normal, normal);
		}
	}

	void MeshPrimitive::tryGenerateSmoothNormals(size_t firstIndex, size_t lastIndex)
	{
		// TODO: implement
		tryGenerateFlatNormals();
	}

	void MeshPrimitive::tryGenerateSmoothNormalsForIndexedTriangles(size_t firstIndex, size_t lastIndex)
	{
		if (indecesAccessor == nullptr)
		{
			return;
		}

		// Clear all normals
		size_t vertecesCount = getVertecesCount();
		for (size_t i = 0; i < vertecesCount; ++i)
		{
			setVertexNormal(i, { 0.0f, 0.0f, 0.0f });
		}

		size_t trianglesCount = getTrianglesCount();
		firstIndex = std::min(firstIndex, trianglesCount - 1);
		lastIndex = std::min(lastIndex, trianglesCount - 1);

		for (size_t i = firstIndex; i <= lastIndex; ++i)
		{
			mathem::TriangleGeometry3D triangle = getTrianglePositions(i);
			mathem::Vector3D normal = triangle.computeNormal();

			uint32_t vertex1;
			uint32_t vertex2;
			uint32_t vertex3;
			getTriangleVertexIndeces(i, &vertex1, &vertex2, &vertex3);

			mathem::Vector3D vertexNormal1 = (normal + getVertexNormal(vertex1));
			mathem::Vector3D vertexNormal2 = (normal + getVertexNormal(vertex2));
			mathem::Vector3D vertexNormal3 = (normal + getVertexNormal(vertex3));
			setVertexNormal(vertex1, vertexNormal1);
			setVertexNormal(vertex2, vertexNormal2);
			setVertexNormal(vertex3, vertexNormal3);
		}

		// Normalize all normals
		for (size_t i = 0; i < vertecesCount; ++i)
		{
			mathem::Vector3D vertexNormal = getVertexNormal(i);
			vertexNormal.normalize();
			setVertexNormal(i, vertexNormal);
		}
	}

	void MeshPrimitive::initVerteces(
		size_t count,
		bool normals,
		bool textureCoords_0,
		bool textureCoords_1,
		bool colors_0,
		bool joints_0,
		bool weights_0,
		bool tangents)
	{
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

	uint32_t MeshPrimitive::getVertecesCount()
	{
		return getPositions()->count;
	}
	
	mathem::Vector3D MeshPrimitive::getVertexPosition(uint32_t index)
	{
		return getPositions()->getVector3D(index);
	}

	mathem::Vector3D MeshPrimitive::getVertexNormal(uint32_t index)
	{
		return getNormals()->getVector3D(index);
	}

	mathem::Vector2D MeshPrimitive::getVertexTextureCoords_0(uint32_t index)
	{
		return getTextureCoords_0()->getVector2D(index);
	}

	mathem::Vector2D MeshPrimitive::getVertexTextureCoords_1(uint32_t index)
	{
		return getTextureCoords_1()->getVector2D(index);
	}

	void MeshPrimitive::setVertexPosition(uint32_t index, mathem::Vector3D position)
	{
		getPositions()->setVector3D(index, position);
	}

	void MeshPrimitive::setVertexNormal(uint32_t index, mathem::Vector3D normal)
	{
		getNormals()->setVector3D(index, normal);
	}

	size_t MeshPrimitive::appendVertex()
	{
		size_t vertecesCount = getVertecesCount();
		resizeVerteces(vertecesCount + 1);
		return vertecesCount;
	}

	bool MeshPrimitive::containsVertexPosition(mathem::Vector3D position)
	{
		size_t vertecesCount = getVertecesCount();
		for (size_t i = 0; i < vertecesCount; ++i)
		{
			if (getVertexPosition(i) == position)
			{
				return true;
			}
		}
		return false;
	}

	void MeshPrimitive::reserveIndexedTriangles(size_t count)
	{
		getIndeces()->reserveElements(count * 3);
	}

	void MeshPrimitive::resizeIndexedTriangles(size_t count)
	{
		getIndeces()->resizeElements(count * 3);
	}

	uint32_t MeshPrimitive::getIndecesCount()
	{
		return getIndeces()->count;
	}

	void MeshPrimitive::removeIndexedTriangleSwapWithLast(size_t index)
	{
		size_t lastIndex = getTrianglesCount() - 1;
		if (index != lastIndex)
		{
			uint32_t lastVertex1;
			uint32_t lastVertex2;
			uint32_t lastVertex3;
			getTriangleVertexIndeces(lastIndex, &lastVertex1, &lastVertex2, &lastVertex3);
			setIndexedTriangle(index, lastVertex1, lastVertex2, lastVertex3);
		}
		removeLastIndexedTriangle();
	}

	void MeshPrimitive::removeLastIndexedTriangle()
	{
		size_t indecesCount = getIndeces()->count;
		if (indecesCount < 3)
		{
			getIndeces()->resizeElements(0);
		}
		else
		{
			getIndeces()->resizeElements(indecesCount - 3);
		}
	}

	void MeshPrimitive::setIndexedTriangle(size_t index, size_t vertexIndex1, size_t vertexIndex2, size_t vertexIndex3)
	{
		size_t vertexIndex = index * 3;
		getIndeces()->setUint(vertexIndex, vertexIndex1);
		getIndeces()->setUint(vertexIndex + 1, vertexIndex2);
		getIndeces()->setUint(vertexIndex + 2, vertexIndex3);
	}

	void MeshPrimitive::appendIndexedTriangle(size_t vertexIndex1, size_t vertexIndex2, size_t vertexIndex3)
	{
		size_t indecesCount = getIndeces()->count;
		getIndeces()->resizeElements(indecesCount + 3);
		getIndeces()->setUint(indecesCount, vertexIndex1);
		getIndeces()->setUint(indecesCount + 1, vertexIndex2);
		getIndeces()->setUint(indecesCount + 2, vertexIndex3);
	}

	void MeshPrimitive::removeVertexSwapWithLast(size_t index)
	{
		size_t lastIndex = getVertecesCount() - 1;
		if (index != lastIndex)
		{
			mathem::Vector3D position = getVertexPosition(lastIndex);
			setVertexPosition(index, position);
			// TODO: swap other attributes
		}
		resizeVerteces(lastIndex);
	}

	bool MeshPrimitive::removeUnusedVertex()
	{
		size_t vertecesCount = getVertecesCount();
		size_t trianglesCount = getTrianglesCount();

		// Find unused vertex
		size_t i = 0;
		bool allAreUsefull = false;
		for (; i < vertecesCount; ++i)
		{
			allAreUsefull = false;
			for (size_t triangleIndex = 0; triangleIndex < trianglesCount; ++triangleIndex)
			{
				size_t vertexIndex1 = ~0;
				size_t vertexIndex2 = ~0;
				size_t vertexIndex3 = ~0;
				getTriangleVertexIndeces(triangleIndex, &vertexIndex1, &vertexIndex2, &vertexIndex3);
				if (vertexIndex1 == i ||
					vertexIndex2 == i ||
					vertexIndex3 == i)
				{
					allAreUsefull = true;
					break;
				}
			}

			if (allAreUsefull == false)
			{
				break;
			}
		}

		if (allAreUsefull)
		{
			return false;
		}

		std::cout << "removing " << i << " vertex" << std::endl;
		removeVertexSwapWithLast(i);

		// Update vertex indeces
		for (size_t triangleIndex = 0; triangleIndex < trianglesCount; ++triangleIndex)
		{
			size_t vertexIndex1 = ~0;
			size_t vertexIndex2 = ~0;
			size_t vertexIndex3 = ~0;
			getTriangleVertexIndeces(triangleIndex, &vertexIndex1, &vertexIndex2, &vertexIndex3);

			if (vertexIndex1 == (vertecesCount - 1))
			{
				vertexIndex1 = i;
			}

			if (vertexIndex2 == (vertecesCount - 1))
			{
				vertexIndex2 = i;
			}

			if (vertexIndex3 == (vertecesCount - 1))
			{
				vertexIndex3 = i;
			}

			setIndexedTriangle(triangleIndex, vertexIndex1, vertexIndex2, vertexIndex3);
		}
		return true;
	}

	void MeshPrimitive::removeUnusedVerteces()
	{
		while (removeUnusedVertex());
	}

	uint32_t MeshPrimitive::getVertecesOrIndecesCount()
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
		initVerteces(count * 3, normals, textureCoords_0, textureCoords_1, colors_0, joints_0, weights_0, tangents);
	}

	void MeshPrimitive::initIndexedTriangles(
		size_t count,
		ComponentDataType indexDataType,
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
		initVerteces(count, normals, textureCoords_0, textureCoords_1, colors_0, joints_0, weights_0, tangents);
		indecesAccessor = new BufferViewAccessor();
		safety::SafeByteArray* indecesBuffer = new safety::SafeByteArray();
		indecesAccessor->init(indecesBuffer, getDataTypeSize(indexDataType), 0, indexDataType, BufferViewAccessor::Type::SCALAR);
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

	uint32_t MeshPrimitive::getTrianglesCount()
	{
		uint32_t pointsCount = getVertecesOrIndecesCount();

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
		if (indecesAccessor)
		{
			uint32_t vertexIndex = triangleIndex * 3;
			*vertex1 = indecesAccessor->getUint(vertexIndex);
			*vertex2 = indecesAccessor->getUint(vertexIndex + 1);
			*vertex3 = indecesAccessor->getUint(vertexIndex + 2);
			return;
		}

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
		resultTriangle.getPoint1() = getVertexPosition(vertex1);
		resultTriangle.getPoint2() = getVertexPosition(vertex2);
		resultTriangle.getPoint3() = getVertexPosition(vertex3);
		return resultTriangle;
	}

	mathem::TriangleGeometry3D MeshPrimitive::getTriangleNormals(uint32_t index)
	{
		mathem::TriangleGeometry3D resultTriangle;
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		resultTriangle.getPoint1() = getVertexNormal(vertex1);
		resultTriangle.getPoint2() = getVertexNormal(vertex2);
		resultTriangle.getPoint3() = getVertexNormal(vertex3);
		return resultTriangle;
	}

	mathem::TriangleGeometry2D MeshPrimitive::getTriangleTextureCoords_0(uint32_t index)
	{
		mathem::TriangleGeometry2D resultTriangle;
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		resultTriangle.getPoint1() = getVertexTextureCoords_0(vertex1);
		resultTriangle.getPoint2() = getVertexTextureCoords_0(vertex2);
		resultTriangle.getPoint3() = getVertexTextureCoords_0(vertex3);
		return resultTriangle;
	}

	mathem::TriangleGeometry2D MeshPrimitive::getTriangleTextureCoords_1(uint32_t index)
	{
		mathem::TriangleGeometry2D resultTriangle;
		uint32_t vertex1, vertex2, vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		resultTriangle.getPoint1() = getVertexTextureCoords_1(vertex1);
		resultTriangle.getPoint2() = getVertexTextureCoords_1(vertex2);
		resultTriangle.getPoint3() = getVertexTextureCoords_1(vertex3);
		return resultTriangle;
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

	size_t MeshPrimitive::appendTriangle(mathem::TriangleGeometry3D positions)
	{
		size_t trianglesCount = getTrianglesCount();
		appendTopologyElements(1);
		setTrianglePositions(trianglesCount, positions.getPoint1(), positions.getPoint2(), positions.getPoint3());
		return trianglesCount + 1;
	}

	size_t MeshPrimitive::appendTriangle_Normal(mathem::TriangleGeometry3D positions, mathem::TriangleGeometry3D normals)
	{
		size_t trianglesCount = getTrianglesCount();
		appendTopologyElements(1);
		setTrianglePositions(trianglesCount, positions.getPoint1(), positions.getPoint2(), positions.getPoint3());
		setTriangleNormals(trianglesCount, normals.getPoint1(), normals.getPoint2(), normals.getPoint3());
		return trianglesCount + 1;
	}

	void MeshPrimitive::removeTriangleSwapWithLast(size_t index)
	{
		uint32_t vertex1;
		uint32_t vertex2;
		uint32_t vertex3;
		getTriangleVertexIndeces(index, &vertex1, &vertex2, &vertex3);
		size_t trianglesCount = getTrianglesCount();
		uint32_t lastVertex1;
		uint32_t lastVertex2;
		uint32_t lastVertex3;
		getTriangleVertexIndeces(trianglesCount - 1, &lastVertex1, &lastVertex2, &lastVertex3);

		// Swap positions
		mathem::Vector3D vector1 = getVertexPosition(lastVertex1);
		mathem::Vector3D vector2 = getVertexPosition(lastVertex2);
		mathem::Vector3D vector3 = getVertexPosition(lastVertex3);
		setVertexPosition(vertex1, vector1);
		setVertexPosition(vertex2, vector2);
		setVertexPosition(vertex3, vector3);

		// Swap normals
		vector1 = getVertexNormal(lastVertex1);
		vector2 = getVertexNormal(lastVertex2);
		vector3 = getVertexNormal(lastVertex3);
		setVertexNormal(vertex1, vector1);
		setVertexNormal(vertex2, vector2);
		setVertexNormal(vertex3, vector3);

		// TODO: Swap other attributes

		removeLastTopologyElement();
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

	size_t MeshPrimitive::toTopologyElementsCount(size_t vertecesCount)
	{
		if (vertecesCount == 0)
		{
			return 0;
		}

		switch (topologyType)
		{
		case mitevox::POINTS:
			return vertecesCount;
			break;
		case mitevox::LINES:
			return vertecesCount / 2;
			break;
		case mitevox::LINE_LOOP:
		case mitevox::LINE_STRIP:
			return vertecesCount - 1;
			break;
		case mitevox::TRIANGLES:
			return vertecesCount / 3;
			break;
		case mitevox::TRIANGLE_STRIP:
		case mitevox::TRIANGLE_FAN:
			return vertecesCount - 2;
			break;
		default:
			break;
		}
		return 0;
	}
}