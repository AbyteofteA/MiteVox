#ifndef OCTREENODE_H
#define OCTREENODE_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkCollision.h"

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
		std::vector<T*> dataPoints;

		inline OctreeNode(mathem::Vector3D position, mathem::Vector3D halfSize);
		inline ~OctreeNode();

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
		inline void tryEliminateOverload(OctreeNode<T>* parentNode, const size_t nodeCapacity, float nodeMinHalfSize);

		inline void tryCreateSubnode(size_t subNodeIndex);
		inline void tryDeleteSubnode(size_t subNodeIndex);
		inline OctreeNode<T>* trySuggestSubnode(T* datapoint, const float nodeMinHalfSize);

		inline void emplace(T* datapoint, const size_t nodeCapacity, const float nodeMinHalfSize);
		inline void clear();
		inline void getAll(safety::SafeArray<T*>* datapointsStack);

		/// <summary>
		/// Cuts off empty leaves, subdevides overloaded leaves.
		/// </summary>
		/// <param name="nodeCapacity"> - max amount of OctreeDataPoints per node</param>
		/// <param name="nodeMaxLifeTime"></param>
		/// <param name="nodeMinHalfSize"></param>
		/// <returns>true if the node needs to be deleted, false otherwise</returns>
		inline bool update(const size_t nodeCapacity, const size_t nodeMaxLifeTime, const float nodeMinHalfSize);
	};


	// IMPLEMENTATION BELOW //


	template <class T>
	inline OctreeNode<T>::OctreeNode(mathem::Vector3D position, mathem::Vector3D halfSize)
	{
		boundingBox.position = position;
		boundingBox.halfSize = halfSize;
	}

	template <class T>
	inline OctreeNode<T>::~OctreeNode()
	{
		for (size_t i = 0; i < 8; i++)
		{
			if (subNodes[i] != nullptr)
			{
				delete subNodes[i];
				subNodes[i] = nullptr;
			}
		}
		dataPoints.clear();
	}

	template <class T>
	inline void OctreeNode<T>::collapse()
	{
		for (size_t i = 0; i < 8; i++)
		{
			OctreeNode<T>* subNode = subNodes[i];
			if (subNode == nullptr)
			{
				continue;
			}

			subNode->collapse();

			T* tmpDataPoint;
			for (size_t j = 0; j < subNode->dataPoints.size(); j++)
			{
				tmpDataPoint = subNode->dataPoints.at(j);
				dataPoints.push_back(tmpDataPoint);
			}

			delete subNode;
			subNodes[i] = nullptr;
		}
	}

	template <class T>
	inline bool OctreeNode<T>::isEmpty()
	{
		if (this->isLeaf())
		{
			return dataPoints.size() == 0;
		}
		else
		{
			for (size_t i = 0; i < 8; i++)
			{
				if (subNodes[i]->isEmpty() == false)
				{
					return false;
				}
			}
			return true;
		}
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
		if (dataPoints.size() > nodeCapacity)
		{
			return dataPoints.size() - nodeCapacity;
		}
		return 0;
	}

	template <class T>
	inline void OctreeNode<T>::tryEliminateOverload(OctreeNode<T>* parentNode, const size_t nodeCapacity, float nodeMinHalfSize)
	{
		size_t nodeOverloadedCapacity = getOverloadCount(nodeCapacity);
		for (size_t i = 0; i < nodeOverloadedCapacity; i++)
		{
			T* dataPoint = dataPoints.back();
			dataPoints.pop_back();
			this->emplace(dataPoint, nodeCapacity, nodeMinHalfSize);
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
			subNodes[subNodeIndex] = new OctreeNode<T>(newOrigin, newHalfSize);
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

		// Nothing to delete
		if (subNodes[subNodeIndex] == nullptr)
		{
			return;
		}

		subNodes[subNodeIndex]->clear();
		delete subNodes[subNodeIndex];
		subNodes[subNodeIndex] = nullptr;
	}

	template <class T>
	inline OctreeNode<T>* OctreeNode<T>::trySuggestSubnode(T* datapoint, float nodeMinHalfSize)
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

		// Check if the datapoint fits into new subnode
		CollisionType ñollisionType = checkCollision(
			datapoint->getCollider(),
			datapoint->getTransform(),
			&subNodes[octant]->boundingBox);
		if (ñollisionType == CollisionType::INSCRIBTION_2_1)
		{
			return subNodes[octant];
		}

		return nullptr;
	}

	template <class T>
	inline void OctreeNode<T>::emplace(T* datapoint, const size_t nodeCapacity, const float nodeMinHalfSize)
	{
		if (dataPoints.size() < nodeCapacity)
		{
			dataPoints.push_back(datapoint);
			return;
		}

		OctreeNode<T>* octant = trySuggestSubnode(datapoint, nodeMinHalfSize);
		if (octant)
		{
			octant->emplace(datapoint, nodeCapacity, nodeMinHalfSize);
		}
		else
		{
			dataPoints.push_back(datapoint); // Overload may occure here
		}
	}

	template <class T>
	inline void OctreeNode<T>::clear()
	{
		dataPoints.clear();

		for (size_t i = 0; i < 8; i++)
		{
			tryDeleteSubnode(i);
		}
	}

	template <class T>
	inline void OctreeNode<T>::getAll(safety::SafeArray<T*>* datapointsStack)
	{
		datapointsStack->concatenate(&dataPoints);

		for (size_t i = 0; i < 8; i++)
		{
			subNodes[i]->getAll(datapointsStack);
		}
	}

	template <class T>
	inline bool OctreeNode<T>::update(const size_t nodeCapacity, const size_t nodeMaxLifeTime, const float nodeMinHalfSize)
	{
		if (this->isLeaf())
		{
			size_t dataPointsCount = dataPoints.size();
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
				tryEliminateOverload(this, nodeCapacity, nodeMinHalfSize);
			}
		}
		else
		{
			tryEliminateOverload(this, nodeCapacity, nodeMinHalfSize);

			// Update all subnodes
			for (size_t i = 0; i < 8; i++)
			{
				OctreeNode<T>* subNode = subNodes[i];
				if (subNode == nullptr)
				{
					continue;
				}

				if (bool needToDelete = subNode->update(nodeCapacity, nodeMaxLifeTime, nodeMinHalfSize))
				{
					tryDeleteSubnode(i);
				}
			}
		}

		return false;
	}
}

#endif