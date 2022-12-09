#ifndef OCTREE_H
#define OCTREE_H

#include "engine/Math/src/DataStructures/Graphs/OctreeNode.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Vector.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Generators/UniqueIDGenerator.h"

#include <vector>
#include <unordered_map>

namespace mathem
{
	/// <summary>
	/// The tree only sorts datapoints spaciallyand updates its structure when the datapoints move.
	/// Simulation code must be abstracted away from the Octree.
	/// Use cases:
	/// - sorting game objects
	/// - sorting mesh polygons
	/// - gravity simulations
	/// 
	/// Datapoint type must define the following methods:
	/// - GeometryTransform* getTransform();
	/// - ComplexGeometry* getCollider();
	/// </summary>
	/// <typeparam name="T">Datapoint type</typeparam>
	template <class T>
	class Octree
	{
	public:

		OctreeNode<T>* origin = nullptr;

		inline Octree(
			float halfSize, 
			float nodeMinHalfSize,
			size_t nodeCapacity, 
			size_t nodeMaxLifeTime);
		inline ~Octree();

		inline void emplace(T* datapoint);
		inline void emplace(safety::SafeArray<T*>* datapointsStack); // TODO: not used
		// TODO: inline OctreeNode<T>* getNearestNode(mathem::Vector3D point);
		inline void getAll(safety::SafeArray<T*>* datapointsStack);
		inline void clear();
		inline void update();

		// TODO: void moveDatapoint();

	private:

		size_t nodeCapacity = 4; // Max amount of OctreeDataPoints per node.
		size_t nodeMaxLifeTime;
		float nodeMinHalfSize;
	};


	// IMPLEMENTATION BELOW //


	template <class T>
	inline Octree<T>::Octree(
		float halfSize,
		float nodeMinHalfSize,
		size_t nodeCapacity,
		size_t nodeMaxLifeTime)
	{
		// TODO: ERROR: vtable crashes here !!!
		this->origin = new OctreeNode<T>({ 0.0f, 0.0f, 0.0f }, { halfSize, halfSize, halfSize });
		this->nodeMinHalfSize = nodeMinHalfSize;
		this->nodeCapacity = nodeCapacity;
		this->nodeMaxLifeTime = nodeMaxLifeTime;
	}

	template <class T>
	inline Octree<T>::~Octree()
	{
		origin->clear();
		delete origin;
	}

	template <class T>
	inline void Octree<T>::emplace(T* datapoint)
	{
		origin->emplace(datapoint, nodeCapacity, nodeMinHalfSize);
	}

	template <class T>
	inline void Octree<T>::emplace(safety::SafeArray<T*>* datapointsStack)
	{
		size_t datapointsElementsCount = datapointsStack->getElementsCount();
		for (size_t i = 0; i < datapointsElementsCount; i++)
		{
			origin->emplace(datapointsStack->getElement(i), nodeCapacity, nodeMinHalfSize);
		}
	}

	template <class T>
	inline void Octree<T>::getAll(safety::SafeArray<T*>* datapointsStack)
	{
		origin->getAll(datapointsStack);
	}

	template <class T>
	inline void Octree<T>::clear()
	{
		origin->clear();
	}

	template <class T>
	inline void Octree<T>::update()
	{
		origin->update(nodeCapacity, nodeMaxLifeTime, nodeMinHalfSize);
	}
}

#endif
