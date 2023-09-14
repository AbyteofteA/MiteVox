#include "makeConvexHullFromMesh.h"

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/PlaneGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"
#include "engine/Math/src/Geometry/GeometryGenerators/makeTetrahedron.h"
#include "engine/Math/src/Geometry/GeometryUtilities/computeProjection.h"
#include "engine/Math/src/Utilities/almostEqual.h"
#include "engine/Math/src/Algorithms/Sorting/bubbleSort.h"

#include "decimateMesh.h"

#include <cstdint>
#include <iostream>
#include <limits>
#include <cassert>

namespace mathem
{
	struct TriangleNeighborsInfo
	{
		size_t count = 0;
		size_t indeces[3] = { 0, 0, 0 };
		size_t edges[3][2]; // 3 neighbors, 2 points for each
	};

	bool tryMakeInitialSimplex(mitevox::Mesh* mesh, mitevox::MeshPrimitive* convexHull)
	{
		size_t vertecesCount = mesh->getVertecesCount();
		if (vertecesCount < 4)
		{
			return false;
		}

		int32_t maxX = 0;
		Vector3D maxPositionX;
		maxPositionX.setMin();

		int32_t minX = 0;
		Vector3D minPositionX;
		minPositionX.setMax();

		int32_t maxY = 0;
		Vector3D maxPositionY;
		maxPositionY.setMin();

		int32_t maxZ = 0;
		Vector3D maxPositionZ;
		maxPositionZ.setMin();

		int32_t minZ = 0;
		Vector3D minPositionZ;
		minPositionZ.setMax();

		for (size_t i = 0; i < vertecesCount; ++i)
		{
			Vector3D vertex = mesh->getVertexPosition(i);

			if (vertex.x() > maxPositionX.x())
			{
				maxX = i;
				maxPositionX = vertex;
			}

			if (vertex.x() < minPositionX.x())
			{
				minX = i;
				minPositionX = vertex;
			}

			if (vertex.y() > maxPositionY.y())
			{
				maxY = i;
				maxPositionY = vertex;
			}

			if (vertex.z() > maxPositionZ.z())
			{
				maxZ = i;
				maxPositionZ = vertex;
			}

			if (vertex.z() < minPositionZ.z())
			{
				minZ = i;
				minPositionZ = vertex;
			}
		}

		size_t forthPointIndex = maxZ;
		Vector3D forthPoint = maxPositionZ;
		if (maxY == maxZ)
		{
			forthPointIndex = minZ;
			forthPoint = minPositionZ;
		}

		return makeTetrahedron(maxPositionX, minPositionX, maxPositionY, forthPoint, convexHull);
	}

	/// <summary>
	/// Updates triangle connections only for visible triangles
	/// </summary>
	void updateTriangleConnections(
		mitevox::MeshPrimitive* convexHull,
		safety::SafeArray<size_t>* visibleTriangles,
		safety::SafeArray<TriangleNeighborsInfo>* trianglesNeighborsInfo)
	{
		trianglesNeighborsInfo->clear();

		size_t trianglesCount = convexHull->getTrianglesCount();
		size_t visibleTrianglesCount = visibleTriangles->getElementsCount();
		for (size_t i = 0; i < visibleTrianglesCount; ++i)
		{
			size_t triangleIndex = visibleTriangles->getElement(i);

			size_t vertex1 = ~0;
			size_t vertex2 = ~0;
			size_t vertex3 = ~0;
			convexHull->getTriangleVertexIndeces(triangleIndex, &vertex1, &vertex2, &vertex3);

			TriangleNeighborsInfo triangleNeighbors;
			triangleNeighbors.count = 0;

			for (size_t neighbor = 0; neighbor < trianglesCount; ++neighbor)
			{
				// Skip the same triangle
				if (triangleIndex == neighbor)
				{
					continue;
				}

				if (triangleNeighbors.count >= 3)
				{
					break;
				}

				size_t neighborVertex1 = ~0;
				size_t neighborVertex2 = ~0;
				size_t neighborVertex3 = ~0;
				convexHull->getTriangleVertexIndeces(neighbor, &neighborVertex1, &neighborVertex2, &neighborVertex3);

				size_t edgeIndex = 0;
				if (vertex1 == neighborVertex1 ||
					vertex1 == neighborVertex2 ||
					vertex1 == neighborVertex3)
				{
					triangleNeighbors.edges[triangleNeighbors.count][edgeIndex] = vertex1;
					edgeIndex += 1;
				}

				if (vertex2 == neighborVertex1 ||
					vertex2 == neighborVertex2 ||
					vertex2 == neighborVertex3)
				{
					triangleNeighbors.edges[triangleNeighbors.count][edgeIndex] = vertex2;
					edgeIndex += 1;
				}

				if (vertex3 == neighborVertex1 ||
					vertex3 == neighborVertex2 ||
					vertex3 == neighborVertex3)
				{
					if (edgeIndex >= 2)
					{
						std::cout << "ERROR: edgeIndex >= 2" << std::endl;
						assert((edgeIndex < 2));
						continue;
					}

					triangleNeighbors.edges[triangleNeighbors.count][edgeIndex] = vertex3;
					edgeIndex += 1;
				}

				if (edgeIndex >= 2)
				{
					triangleNeighbors.indeces[triangleNeighbors.count] = neighbor;
					triangleNeighbors.count += 1;
				}
			}

			trianglesNeighborsInfo->appendElement(triangleNeighbors);
		}
	}

	void updateVisibleTrianglesFormPoint(
		Vector3D point, 
		mitevox::MeshPrimitive* convexHull, 
		safety::SafeArray<TriangleNeighborsInfo>* trianglesNeighborsInfo, 
		size_t visibleTriangle, 
		safety::SafeArray<size_t>* visibleTriangles)
	{
		visibleTriangles->clear();

		size_t trianglesCount = convexHull->getTrianglesCount();
		for (size_t i = 0; i < trianglesCount; ++i)
		{
			TriangleGeometry3D triangle = convexHull->getTrianglePositions(i);
			PlaneGeometry plane = triangle.computePlane();
			if (plane.isBeforePlane(point))
			{
				visibleTriangles->appendElement(i);
			}
		}
	}

	Vector3D computeVisibleTrianglesCentroid(mitevox::MeshPrimitive* convexHull, safety::SafeArray<size_t>* visibleTriangles)
	{
		Vector3D centroid = Vector3D::zero();

		size_t visibleTrianglesCount = visibleTriangles->getElementsCount();
		for (size_t i = 0; i < visibleTrianglesCount; ++i)
		{
			TriangleGeometry3D triangle = convexHull->getTrianglePositions(i);
			Vector3D triangleCentroid = triangle.computeCentroid();
			centroid += triangleCentroid;
		}

		return centroid / visibleTrianglesCount;
	}

	void computeHorizonEdges(
		mitevox::MeshPrimitive* convexHull, 
		safety::SafeArray<size_t>* visibleTriangles, 
		safety::SafeArray<TriangleNeighborsInfo>* trianglesNeighborsInfo, 
		safety::SafeArray<size_t>* horizonEdges)
	{
		horizonEdges->clear();

		size_t visibleTrianglesCount = visibleTriangles->getElementsCount();
		for (size_t i = 0; i < visibleTrianglesCount; ++i)
		{
			size_t visibleTriangleIndex = i;
			TriangleNeighborsInfo triangleNeighborsInfo = trianglesNeighborsInfo->getElement(visibleTriangleIndex);

			bool hasRepetitions = false;
			bool hasVisibleNeighbor = false;
			for (size_t neighbor = 0; neighbor < triangleNeighborsInfo.count; ++neighbor)
			{
				if (neighbor == 1)
				{
					if (triangleNeighborsInfo.indeces[1] == triangleNeighborsInfo.indeces[0])
					{
						hasRepetitions = true;
					}
				}
				else if (neighbor == 2)
				{
					if (triangleNeighborsInfo.indeces[2] == triangleNeighborsInfo.indeces[0] ||
						triangleNeighborsInfo.indeces[2] == triangleNeighborsInfo.indeces[1])
					{
						hasRepetitions = true;
					}
				}

				if (visibleTriangles->contains(triangleNeighborsInfo.indeces[neighbor]) == false)
				{
					horizonEdges->appendElement(triangleNeighborsInfo.edges[neighbor][0]);
					horizonEdges->appendElement(triangleNeighborsInfo.edges[neighbor][1]);
				}
				else
				{
					hasVisibleNeighbor = true;
				}
			}

			bool isValid = true;
			if (visibleTrianglesCount != 1 &&
				hasVisibleNeighbor == false )
			{
				std::cout << "ERROR: hasVisibleNeighbor == false" << std::endl;
				isValid == false;
			}
			if (triangleNeighborsInfo.count != 3)
			{
				std::cout << "ERROR: triangleNeighborsInfo.count != 3" << std::endl;
				assert((triangleNeighborsInfo.count == 3));
			}
			assert((hasRepetitions == false));
			assert(isValid);
		}
	}

	void constructUmbrella(mitevox::MeshPrimitive* convexHull, Vector3D point, safety::SafeArray<size_t>* horizonEdges, Vector3D umbrellaNormal)
	{
		size_t umbrellaTopVertexIndex = convexHull->appendVertex();
		convexHull->setVertexPosition(umbrellaTopVertexIndex, point);

		size_t newTrianglesCount = horizonEdges->getElementsCount() / 2;
		for (size_t i = 0; i < newTrianglesCount; ++i)
		{
			size_t vertexIndex2 = horizonEdges->getElement(i * 2);
			size_t vertexIndex3 = horizonEdges->getElement(i * 2 + 1);

			TriangleGeometry3D newTriangle;
			newTriangle.getPoint1() = point;
			newTriangle.getPoint2() = convexHull->getVertexPosition(vertexIndex2);
			newTriangle.getPoint3() = convexHull->getVertexPosition(vertexIndex3);

			Vector3D planeNormal = newTriangle.computeNormal();
			if (planeNormal * umbrellaNormal < 0.0f)
			{
				convexHull->appendIndexedTriangle(umbrellaTopVertexIndex, vertexIndex3, vertexIndex2);
			}
			else
			{
				convexHull->appendIndexedTriangle(umbrellaTopVertexIndex, vertexIndex2, vertexIndex3);
			}
		}
	}

	void deleteVisibleTriangles(mitevox::MeshPrimitive* convexHull, safety::SafeArray<size_t>* visibleTriangles)
	{
		size_t visibleTrianglesCount = visibleTriangles->getElementsCount();
		for (size_t i = 0; i < visibleTrianglesCount; ++i)
		{
			size_t visibleTriangleIndex = visibleTriangles->getElement(i);
			convexHull->removeIndexedTriangleSwapWithLast(visibleTriangleIndex);
		}
	}

	bool isVisible(Vector3D point, mitevox::MeshPrimitive* convexHull, size_t* triangle)
	{
		*triangle = ~0;

		size_t trianglesCount = convexHull->getTrianglesCount();
		for (size_t i = 0; i < trianglesCount; ++i)
		{
			PlaneGeometry plane = convexHull->getTrianglePositions(i).computePlane();
			if (plane.isBeforePlane(point))
			{
				*triangle = i;
				return true;
			}
		}

		return false;
	}

	/// <summary>
	/// TODO: why sometimes deletedTriangles > addedTriangles?
	/// </summary>
	/// <param name="mesh"></param>
	/// <param name="convexHull"></param>
	void makeConvexHullFromMeshBruteForce(mitevox::Mesh* mesh, mitevox::MeshPrimitive* convexHull)
	{
		if (tryMakeInitialSimplex(mesh, convexHull) == false)
		{
			return;
		}

		size_t vertecesCount = mesh->getVertecesCount();
		size_t meshTrianglesCount = mesh->getTrianglesCount();

		convexHull->reserveIndexedTriangles(meshTrianglesCount);

		safety::SafeArray<TriangleNeighborsInfo> trianglesNeighborsInfo;
		trianglesNeighborsInfo.reserve(meshTrianglesCount);
		safety::SafeArray<size_t> horizonEdges;
		horizonEdges.reserve(vertecesCount * 2);
		safety::SafeArray<size_t> visibleTriangles;
		visibleTriangles.reserve(meshTrianglesCount);
		
		for (size_t i = 0; i < vertecesCount; ++i)
		{
			size_t triangle = 0;
			Vector3D meshPoint = mesh->getVertexPosition(i);

			if (convexHull->containsVertexPosition(meshPoint))
			{
				continue;
			}

			if (isVisible(meshPoint, convexHull, &triangle) == false)
			{
				//std::cout << "skipping invisible vertex" << std::endl;
				continue;
			}

			updateVisibleTrianglesFormPoint(meshPoint, convexHull, &trianglesNeighborsInfo, triangle, &visibleTriangles);
			updateTriangleConnections(convexHull, &visibleTriangles, &trianglesNeighborsInfo);

			Vector3D visibleTrianglesCentroid = computeVisibleTrianglesCentroid(convexHull, &visibleTriangles);
			Vector3D umbrellaNormal = meshPoint - visibleTrianglesCentroid;
			umbrellaNormal.normalize();

			computeHorizonEdges(convexHull, &visibleTriangles, &trianglesNeighborsInfo, &horizonEdges);
			
			bubbleSort(&visibleTriangles, false);
			deleteVisibleTriangles(convexHull, &visibleTriangles);
			constructUmbrella(convexHull, meshPoint, &horizonEdges, umbrellaNormal);
		
			size_t deletedTriangles = visibleTriangles.getElementsCount();
			size_t addedTriangles = horizonEdges.getElementsCount() / 2;
			if (deletedTriangles > addedTriangles)
			{
				std::cout << "ERROR: deletedTriangles > addedTriangles" << std::endl;
				deletedTriangles = deletedTriangles;
			}
		}

		std::cout << "convex hull triangles count = " << convexHull->getTrianglesCount() << std::endl;
		convexHull->tryGenerateSmoothNormalsForIndexedTriangles();
	}
}