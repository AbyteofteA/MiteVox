#include "decimateMesh.h"

#include "engine/CodeSafety/src/SafeArray.h"

#include <limits>
#include <utility>

namespace mathem
{
	struct EdgeInfo
	{
		size_t point1 = ~0;
		size_t point2 = ~0;
		size_t triangleToRemove1 = ~0;
		size_t triangleToRemove2 = ~0;
	};

	void getSmallestEdge(mitevox::MeshPrimitive* meshPrimitive, size_t* point1, size_t* point2)
	{
		*point1 = ~0;
		*point2 = ~0;
		float minEdgeLength = std::numeric_limits<float>::max();

		size_t trianglesCount = meshPrimitive->getTrianglesCount();
		for (size_t i = 0; i < trianglesCount; ++i)
		{
			size_t vertexIndex1 = ~0;
			size_t vertexIndex2 = ~0;
			size_t vertexIndex3 = ~0;
			meshPrimitive->getTriangleVertexIndeces(i, &vertexIndex1, &vertexIndex2, &vertexIndex3);

			Vector3D vertex1 = meshPrimitive->getVertexPosition(vertexIndex1);
			Vector3D vertex2 = meshPrimitive->getVertexPosition(vertexIndex2);
			Vector3D vertex3 = meshPrimitive->getVertexPosition(vertexIndex3);

			Vector3D tmpVector = vertex2 - vertex1;
			float tmpEdgeLength = tmpVector.getLength();
			if (tmpEdgeLength < minEdgeLength)
			{
				minEdgeLength = tmpEdgeLength;
				*point1 = vertexIndex1;
				*point2 = vertexIndex2;
			}

			tmpVector = vertex3 - vertex1;
			tmpEdgeLength = tmpVector.getLength();
			if (tmpEdgeLength < minEdgeLength)
			{
				minEdgeLength = tmpEdgeLength;
				*point1 = vertexIndex1;
				*point2 = vertexIndex3;
			}

			tmpVector = vertex3 - vertex2;
			tmpEdgeLength = tmpVector.getLength();
			if (tmpEdgeLength < minEdgeLength)
			{
				minEdgeLength = tmpEdgeLength;
				*point1 = vertexIndex2;
				*point2 = vertexIndex3;
			}
		}
	}

	bool getTrianglesWithEdge(
		mitevox::MeshPrimitive* meshPrimitive, 
		size_t edgePoint1, 
		size_t edgePoint2, 
		size_t* triangle1, 
		size_t* triangle2)
	{
		*triangle1 = ~0;
		*triangle2 = ~0;

		size_t trianglesCount = meshPrimitive->getTrianglesCount();
		for (size_t i = 0; i < trianglesCount; ++i)
		{
			size_t vertexIndex1 = ~0;
			size_t vertexIndex2 = ~0;
			size_t vertexIndex3 = ~0;
			meshPrimitive->getTriangleVertexIndeces(i, &vertexIndex1, &vertexIndex2, &vertexIndex3);

			size_t edgeIndex = 0;
			if (vertexIndex1 == edgePoint1 ||
				vertexIndex1 == edgePoint2)
			{
				edgeIndex += 1;
			}

			if (vertexIndex2 == edgePoint1 ||
				vertexIndex2 == edgePoint2)
			{
				edgeIndex += 1;
			}

			if (vertexIndex3 == edgePoint1 ||
				vertexIndex3 == edgePoint2)
			{
				edgeIndex += 1;
			}

			if (edgeIndex > 2)
			{
				assert((edgeIndex <= 2));
			}
			if (edgeIndex != 2)
			{
				continue;
			}

			if (*triangle1 == ~0)
			{
				*triangle1 = i;
			}
			else
			{
				*triangle2 = i;
				return true;
			}
		}
		return false;
	}

	void snapEdgeToCenter(mitevox::MeshPrimitive* meshPrimitive, size_t edgePoint1, size_t edgePoint2)
	{
		Vector3D edgePosition1 = meshPrimitive->getVertexPosition(edgePoint1);
		Vector3D edgePosition2 = meshPrimitive->getVertexPosition(edgePoint2);
		Vector3D edgeCenter = (edgePosition1 + edgePosition2) * 0.5f;

		meshPrimitive->setVertexPosition(edgePoint1, edgeCenter);
		meshPrimitive->setVertexPosition(edgePoint2, edgeCenter);

		size_t trianglesCount = meshPrimitive->getTrianglesCount();
		for (size_t i = 0; i < trianglesCount; ++i)
		{
			size_t vertexIndex1 = ~0;
			size_t vertexIndex2 = ~0;
			size_t vertexIndex3 = ~0;
			meshPrimitive->getTriangleVertexIndeces(i, &vertexIndex1, &vertexIndex2, &vertexIndex3);

			if (vertexIndex1 == edgePoint1 ||
				vertexIndex1 == edgePoint2)
			{
				vertexIndex1 = edgePoint1;
			}

			if (vertexIndex2 == edgePoint1 ||
				vertexIndex2 == edgePoint2)
			{
				vertexIndex2 = edgePoint1;
			}

			if (vertexIndex3 == edgePoint1 ||
				vertexIndex3 == edgePoint2)
			{
				vertexIndex3 = edgePoint1;
			}
			meshPrimitive->setIndexedTriangle(i, vertexIndex1, vertexIndex2, vertexIndex3);
		}
	}

	void decimateMesh(mitevox::MeshPrimitive* meshPrimitive, size_t maxTrianglesCount)
	{
		EdgeInfo edgeInfo;
		while (meshPrimitive->getTrianglesCount() > maxTrianglesCount)
		{
			getSmallestEdge(meshPrimitive, &edgeInfo.point1, &edgeInfo.point2);
			getTrianglesWithEdge(
				meshPrimitive,
				edgeInfo.point1,
				edgeInfo.point2,
				&edgeInfo.triangleToRemove1,
				&edgeInfo.triangleToRemove2);

			if (edgeInfo.triangleToRemove1 < edgeInfo.triangleToRemove2)
			{
				std::swap(edgeInfo.triangleToRemove1, edgeInfo.triangleToRemove2);
			}
			meshPrimitive->removeIndexedTriangleSwapWithLast(edgeInfo.triangleToRemove1);
			meshPrimitive->removeIndexedTriangleSwapWithLast(edgeInfo.triangleToRemove2);

			snapEdgeToCenter(meshPrimitive, edgeInfo.point1, edgeInfo.point2);
		}

		meshPrimitive->removeUnusedVerteces();
	}
}