#ifndef OCTREE_H
#define OCTREE_H

#include "engine/Math/src/DataStructures/Graphs/OctreeNode.h"
#include "engine/Math/src/DataStructures/PileOfContainers.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Vector.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/DataStructures/PileAllocator.h"

namespace mathem
{
	/// <summary>
	/// The tree only sorts datapoints spacially and updates its structure when the datapoints move.
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

		inline void emplaceAtLevel(T datapoint, int32_t level);
		// TODO: inline OctreeNode<T>* getNearestNode(mathem::Vector3D point);
		inline void getAll(safety::SafeArray<T>* datapointsStack);
		inline void clear();
		inline void update(int32_t level, safety::SafeArray<T>* datapointsToMove);

		inline size_t getNodeCapacity();

		inline void getCollisions(safety::SafeArray<CollisionInfo<T>>* collisions, PileOfSafeArrays<T>* dataPointsContainers, float equalityTolerance);

	private:

		size_t nodeCapacity = 4; // Max amount of OctreeDataPoints per node.
		size_t nodeMaxLifeTime;
		float nodeMinHalfSize;

		PileAllocator<OctreeNode<T>> nodesAllocator;
	};


	// IMPLEMENTATION BELOW //


	template <class T>
	inline Octree<T>::Octree(
		float halfSize,
		float nodeMinHalfSize,
		size_t nodeCapacity,
		size_t nodeMaxLifeTime)
	{
		OctreeNode<T>::nodesAllocator = &nodesAllocator;
		this->origin = nodesAllocator.useElement();
		this->origin->setBoundingBox({ 0.0f, 0.0f, 0.0f }, { halfSize, halfSize, halfSize });
		this->nodeMinHalfSize = nodeMinHalfSize;
		this->nodeCapacity = nodeCapacity;
		this->nodeMaxLifeTime = nodeMaxLifeTime;

		nodesAllocator.allocate(256); // TODO: add to EngineSettings
	}

	template <class T>
	inline Octree<T>::~Octree()
	{
		origin->clear();
		delete origin;
	}

	template <class T>
	inline void Octree<T>::emplaceAtLevel(T datapoint, int32_t level)
	{
		origin->emplaceAtLevel(datapoint, level, nodeCapacity, nodeMinHalfSize);
	}

	template <class T>
	inline void Octree<T>::getAll(safety::SafeArray<T>* datapointsStack)
	{
		origin->getAll(datapointsStack);
	}

	template <class T>
	inline void Octree<T>::clear()
	{
		origin->clear();
	}

	template <class T>
	inline void Octree<T>::update(int32_t level, safety::SafeArray<T>* datapointsToMove)
	{
		origin->update(origin, level, nodeCapacity, nodeMaxLifeTime, nodeMinHalfSize, datapointsToMove);
	}

	template <class T>
	inline size_t Octree<T>::getNodeCapacity()
	{
		return nodeCapacity;
	}

	template <class T>
	inline void Octree<T>::getCollisions(safety::SafeArray<CollisionInfo<T>>* collisions, PileOfSafeArrays<T>* dataPointsContainers, float equalityTolerance)
	{
		origin->getCollisions(collisions, dataPointsContainers, equalityTolerance);
	}
}

#endif
