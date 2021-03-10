
#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeDataPoint.h"
#include "OctreeNode.h"

#include "engine/Math/src/LinearAlgebra/Point3D.h"
#include "engine/Math/src/Generators/UniqueIDGenerator.h"

#include <vector>
#include <unordered_map>

namespace mathem
{
	template <typename T>
	class Octree
	{
	public:

		OctreeNode<T>* origin;

		Octree(float halfSize, size_t _nodeCapacity);
		~Octree();

		OctreeNode<T>* getNearestNode(mathem::Point3D point);
		IDtype emplace(mathem::Point3D pos, T value);
		void remove(IDtype ID);

		/// <summary>
		/// Cuts off empty leaves, subdevides overloaded leaves.
		/// </summary>
		void update();

	private:

		mathem::UniqueIDGenerator<IDtype> IDGenerator;

		// Stores OctreeDataPoint pointers for quick access.
		std::unordered_map<IDtype, OctreeDataPoint<T>*> dataPoints;

		// Max amount of OctreeDataPoints per node.
		size_t nodeCapacity;

	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	Octree<T>::Octree(float halfSize, size_t _nodeCapacity)
	{
		origin = new OctreeNode<T>(mathem::Point3D(), halfSize);
		nodeCapacity = _nodeCapacity;
	}

	template <typename T>
	Octree<T>::~Octree()
	{
		dataPoints.clear();
		delete origin;
		IDGenerator.reset();
	}

	template <typename T>
	OctreeNode<T>* Octree<T>::getNearestNode(mathem::Point3D point)
	{
		if (origin->isLeaf)
		{
			return this;
		}
		else
		{
			return origin->suggestSubnode(point)->getNearestNode(point);
		}
	}

	template <typename T>
	IDtype Octree<T>::emplace(mathem::Point3D pos, T value)
	{
		IDtype newID = IDGenerator.getID();
		OctreeDataPoint<T>* point = new OctreeDataPoint<T>(newID, pos, value);
		dataPoints[newID] = point;

		origin->emplace(point);
	}

	template <typename T>
	void Octree<T>::remove(IDtype ID)
	{
		OctreeDataPoint<T>* point = dataPoints[ID];

		OctreeNode<T>* tmpNode = point->node;
		tmpNode->remove(ID);
		dataPoints.erase(ID);

		IDGenerator.returnID(ID);
	}

	template <typename T>
	void Octree<T>::update()
	{
		origin->update(nodeCapacity);
	}
}

#endif
