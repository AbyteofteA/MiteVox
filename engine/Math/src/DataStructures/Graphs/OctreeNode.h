#ifndef OCTREENODE_H
#define OCTREENODE_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/CollisionDetection/contains.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkCollision.h"
#include "engine/Math/src/DataStructures/PileOfContainers.h"
#include "engine/Math/src/DataStructures/PileAllocator.h"

#include <vector>
#include <cstdint>

namespace mathem
{
	const float signesX[8] = { -1.f, 1.f, -1.f, 1.f, -1.f, 1.f, -1.f, 1.f };
	const float signesY[8] = { -1.f, -1.f, 1.f, 1.f, -1.f, -1.f, 1.f, 1.f };
	const float signesZ[8] = { -1.f, -1.f, -1.f, -1.f, 1.f, 1.f, 1.f, 1.f };

	template <class T>
	class OctreeNode
	{
	public:

		size_t lifeTime = 0;
		OctreeNode<T>* subNodes[8] = { nullptr };
		AxisAlignedBoxGeometry boundingBox;

		// There lay the dataPoints that fit in the node's bounds and don't fit in the subnodes.
		safety::SafeArray<T> dataPoints;

		static PileAllocator<OctreeNode<T>>* nodesAllocator;

		inline OctreeNode();
		inline ~OctreeNode();

		inline void setBoundingBox(Vector3D position, Vector3D halfSize);

		/// <summary>
		/// Recursively moves data from the subnodes to the node and deletes the subnodes.
		/// </summary>
		inline void collapse();

		/// <summary>
		/// Recursively checks the node and subnodes if they have any OctreeDataPoints.
		/// </summary>
		inline bool isEmpty();

		inline bool isLeaf();

		/// <summary>
		/// Checks if the node has more data than should.
		/// </summary>
		/// <returns></returns>
		inline size_t getOverloadCount(const size_t nodeCapacity);
		inline void tryEliminateOverload(const size_t nodeCapacity, safety::SafeArray<T>* datapointsToMove);

		inline void tryCreateSubnode(size_t subNodeIndex);
		inline void tryDeleteSubnode(size_t subNodeIndex);
		inline OctreeNode<T>* trySuggestSubnode(T datapoint, const float nodeMinHalfSize);

		inline void emplaceAtLevel(T datapoint, int32_t level, const size_t nodeCapacity, const float nodeMinHalfSize);
		inline void clear();
		inline void getAll(safety::SafeArray<T>* datapointsStack);

		inline void getCollisions(safety::SafeArray<CollisionInfo<T>>* collisions, PileOfSafeArrays<T>* dataPointsContainers, float equalityTolerance);

		/// <summary>
		/// Cuts off empty leaves, subdevides overloaded leaves.
		/// </summary>
		/// <param name="nodeCapacity"> - max amount of OctreeDataPoints per node</param>
		/// <param name="nodeMaxLifeTime"></param>
		/// <param name="nodeMinHalfSize"></param>
		/// <returns>true if the node needs to be deleted, false otherwise</returns>
		inline bool update(OctreeNode<T>* origin, int32_t level, const size_t nodeCapacity, const size_t nodeMaxLifeTime, const float nodeMinHalfSize, safety::SafeArray<T>* datapointsToMove);

	private:

		inline void getCollisionsRecursively(
			safety::SafeArray<T>* dataPointsFromParentNodes,
			safety::SafeArray<CollisionInfo<T>>* collisions,
			PileOfSafeArrays<T>* dataPointsContainers, float equalityTolerance);
	};


	// IMPLEMENTATION BELOW //


	template <class T>
	PileAllocator<OctreeNode<T>>* OctreeNode<T>::nodesAllocator;

	template <class T>
	inline OctreeNode<T>::OctreeNode()
	{

	}

	template <class T>
	inline OctreeNode<T>::~OctreeNode()
	{
		clear();
	}

	template <class T>
	inline void OctreeNode<T>::setBoundingBox(Vector3D position, Vector3D halfSize)
	{
		boundingBox.position = position;
		boundingBox.halfSize = halfSize;
	}

	template <class T>
	inline void OctreeNode<T>::collapse()
	{
		for (size_t i = 0; i < 8; i++)
		{
			if (OctreeNode<T>* subNode = subNodes[i])
			{
				subNode->collapse();
				dataPoints.concatenate(subNode->dataPoints);
				tryDeleteSubnode(i);
			}
		}
	}

	template <class T>
	inline bool OctreeNode<T>::isEmpty()
	{
		return dataPoints.getElementsCount() == 0;
	}

	template <class T>
	inline bool OctreeNode<T>::isLeaf()
	{
		for (size_t i = 0; i < 8; ++i)
		{
			if (subNodes[i] != nullptr)
			{
				return false;
			}
		}
		return true;
	}

	template <class T>
	inline size_t OctreeNode<T>::getOverloadCount(const size_t nodeCapacity)
	{
		if (dataPoints.getElementsCount() > nodeCapacity)
		{
			return dataPoints.getElementsCount() - nodeCapacity;
		}
		return 0;
	}

	template <class T>
	inline void OctreeNode<T>::tryEliminateOverload(const size_t nodeCapacity, safety::SafeArray<T>* datapointsToMove)
	{
		size_t nodeOverloadedCapacity = getOverloadCount(nodeCapacity);
		for (size_t i = 0; i < nodeOverloadedCapacity; i++)
		{
			T dataPoint = dataPoints.getLastElement();
			dataPoints.removeLastElement();
			datapointsToMove->appendElement(dataPoint);
		}
	}

	template <class T>
	inline void OctreeNode<T>::tryCreateSubnode(size_t subNodeIndex)
	{
		// CHECK:
		if (subNodeIndex > 7)
		{
			// TODO: log error
			return;
		}

		if (subNodes[subNodeIndex] == nullptr)
		{
			Vector3D newHalfSize = boundingBox.halfSize * 0.5;
			Vector3D newOrigin;
			newOrigin.x() = boundingBox.position.x() + newHalfSize.x() * signesX[subNodeIndex];
			newOrigin.y() = boundingBox.position.y() + newHalfSize.y() * signesY[subNodeIndex];
			newOrigin.z() = boundingBox.position.z() + newHalfSize.z() * signesZ[subNodeIndex];
			subNodes[subNodeIndex] = nodesAllocator->useElement();
			subNodes[subNodeIndex]->setBoundingBox(newOrigin, newHalfSize);
		}
	}

	template <class T>
	inline void OctreeNode<T>::tryDeleteSubnode(size_t subNodeIndex)
	{
		// CHECK:
		if (subNodeIndex > 7)
		{
			// TODO: log error
			return;
		}

		if (OctreeNode<T>* subNode = subNodes[subNodeIndex])
		{
			subNode->clear();
			nodesAllocator->returnUsedElement(subNode);
			subNodes[subNodeIndex] = nullptr;
		}
	}

	template <class T>
	inline OctreeNode<T>* OctreeNode<T>::trySuggestSubnode(T datapoint, float nodeMinHalfSize)
	{
		// Stop subdivision on minimum node size
		if (this->boundingBox.halfSize.x() <= nodeMinHalfSize)
		{
			return nullptr;
		}

		GeometryTransform* datapointTransform = datapoint->getTransform();

		// Choose the subnode (octant) to emplace datapoint based on position
		int octant = 0;
		if (datapointTransform->translation.x() > boundingBox.position.x())
		{
			octant += 1;
		}
		if (datapointTransform->translation.y() > boundingBox.position.y())
		{
			octant += 2;
		}
		if (datapointTransform->translation.z() > boundingBox.position.z())
		{
			octant += 4;
		}

		tryCreateSubnode(octant); // It's not ideal to create it at this point, it may not fit
		return subNodes[octant];
	}

	template <class T>
	inline void OctreeNode<T>::emplaceAtLevel(T datapoint, int32_t level, const size_t nodeCapacity, const float nodeMinHalfSize)
	{
		if (dataPoints.getElementsCount() < nodeCapacity &&
			level <= 0)
		{
			dataPoints.appendElement(datapoint);
			return;
		}

		if (OctreeNode<T>* octant = trySuggestSubnode(datapoint, nodeMinHalfSize))
		{
			// Check if the datapoint fits into new subnode
			GeometryTransform zeroTransform;
			bool datapointFits = contains(
				datapoint->getCollider(),
				datapoint->getTransform(),
				(GeometryPrimitiveBase*)&octant->boundingBox,
				&zeroTransform);
			if (datapointFits)
			{
				octant->emplaceAtLevel(datapoint, level - 1, nodeCapacity, nodeMinHalfSize);
				return;
			}
		}
		dataPoints.appendElement(datapoint); // Overload may occure here
	}

	template <class T>
	inline void OctreeNode<T>::clear()
	{
		lifeTime = 0;
		dataPoints.clear();

		for (size_t i = 0; i < 8; i++)
		{
			tryDeleteSubnode(i);
		}
	}

	template <class T>
	inline void OctreeNode<T>::getAll(safety::SafeArray<T>* datapointsStack)
	{
		datapointsStack->concatenate(&dataPoints);

		for (size_t i = 0; i < 8; i++)
		{
			if (OctreeNode<T>* subNode = subNodes[i])
			{
				subNode->getAll(datapointsStack);
			}
		}
	}

	template <class T>
	inline void OctreeNode<T>::getCollisions(safety::SafeArray<CollisionInfo<T>>* collisions, PileOfSafeArrays<T>* dataPointsContainers, float equalityTolerance)
	{
		safety::SafeArray<T>* newDataPointsContainer = dataPointsContainers->useContainer();
		getCollisionsRecursively(newDataPointsContainer, collisions, dataPointsContainers, equalityTolerance);
	}

	template <class T>
	inline bool OctreeNode<T>::update(
		OctreeNode<T>* origin, 
		int32_t level, 
		const size_t nodeCapacity, 
		const size_t nodeMaxLifeTime, 
		const float nodeMinHalfSize, 
		safety::SafeArray<T>* datapointsToMove)
	{
		// Move datapoints
		size_t dataPointsCount = dataPoints.getElementsCount();
		for (int32_t i = 0; i < dataPointsCount; i++)
		{
			T dataPoint = dataPoints.getElement(i);
			if (dataPoint->getIsMoved())
			{
				GeometryTransform zeroTransform;
				bool datapointFits = contains(
					dataPoint->getCollider(),
					dataPoint->getTransform(),
					(GeometryPrimitiveBase*)&boundingBox,
					&zeroTransform);
				if (datapointFits == false)
				{
					dataPoints.removeElementAndSwapWithLast(i);
					i -= 1;
					dataPointsCount -= 1;
					datapointsToMove->appendElement(dataPoint);
					dataPoint->getTransformForMove();
				}
			}
		}

		if (this->isLeaf())
		{
			if (dataPointsCount == 0)
			{
				lifeTime += 1;

				if (lifeTime > nodeMaxLifeTime)
				{
					return true;
				}
			}
			else
			{
				lifeTime = 0;
				tryEliminateOverload(nodeCapacity, datapointsToMove);
			}
		}
		else
		{
			lifeTime = 0;
			tryEliminateOverload(nodeCapacity, datapointsToMove);

			// Update all subnodes
			for (size_t i = 0; i < 8; i++)
			{
				OctreeNode<T>* subNode = subNodes[i];
				if (subNode == nullptr)
				{
					continue;
				}

				if (bool needToDelete = subNode->update(origin, level, nodeCapacity, nodeMaxLifeTime, nodeMinHalfSize, datapointsToMove))
				{
					tryDeleteSubnode(i);
				}
			}
		}

		return false;
	}

	template <class T>
	inline void OctreeNode<T>::getCollisionsRecursively(
		safety::SafeArray<T>* dataPointsFromParentNodes,
		safety::SafeArray<CollisionInfo<T>>* collisions,
		PileOfSafeArrays<T>* dataPointsContainers,
		float equalityTolerance)
	{
		// dataPoints[i] vs dataPointsFromParentNodes
		size_t dataPointsCount = dataPoints.getElementsCount();
		for (size_t i = 0; i < dataPointsCount; i++)
		{
			size_t dataPointsFromParentNodesCount = dataPointsFromParentNodes->getElementsCount();
			for (size_t j = 0; j < dataPointsFromParentNodesCount; j++)
			{
				CollisionInfo<T> collisionInfo;
				if (checkCollision<T>(dataPoints.getElement(i), dataPointsFromParentNodes->getElement(j), &collisionInfo, equalityTolerance) != CollisionType::NONE)
				{
					collisions->appendElement(collisionInfo);
				}
			}
		}
		// dataPoints[i] vs dataPoints[i]
		checkCollisions<T>(&dataPoints, collisions, equalityTolerance);
		
		dataPointsFromParentNodes->concatenate(&dataPoints);

		for (size_t i = 0; i < 8; i++)
		{
			if (OctreeNode<T>* subNode = subNodes[i])
			{
				subNode->getCollisionsRecursively(dataPointsFromParentNodes, collisions, dataPointsContainers, equalityTolerance);
				safety::SafeArray<T>* previousDataPointsFromParentNodes = dataPointsFromParentNodes;
				dataPointsFromParentNodes = dataPointsContainers->useContainer();
				previousDataPointsFromParentNodes->copyTo(dataPointsFromParentNodes);
			}
		}
	}
}

#endif