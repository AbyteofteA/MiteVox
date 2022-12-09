#ifndef SCENEFOUNDATION_H
#define SCENEFOUNDATION_H

#include "engine/Math/src/DataStructures/Graphs/Octree.h"
#include "engine/MiteVox/src/Playground/Node.h"

#include <cstdint>

namespace mitevox
{
	class SceneFoundation
	{
		friend class Engine;

	public:

		SceneFoundation(
			float halfSize,
			float nodeMinHalfSize,
			size_t nodeCapacity,
			size_t nodeMaxLifeTime,
			size_t chunkLevel,
			size_t maxEmplacedDatapointsPerUpdate);
		~SceneFoundation();

		/// <summary>
		/// Cuts off empty leaves, subdevides overloaded leaves.
		/// </summary>
		void emplace(Node* node);
		void emplace(safety::SafeArray<Node*>* nodesStack);
		void getAll(safety::SafeArray<Node*>* nodesStack);
		void update();
		// TODO: safety::SafeArray<mathem::OctreeNode<Node>>* getNearestChunks(point, radius);

	private:

		mathem::Octree<Node>* octree = nullptr;
		size_t chunkLevel = 0;
		size_t maxEmplacedDatapointsPerUpdate = 64;
		safety::SafeArray<Node*> waitingDatapointsStack;
	};
}

#endif