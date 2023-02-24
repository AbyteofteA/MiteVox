#ifndef SCENEFOUNDATION_H
#define SCENEFOUNDATION_H

#include "engine/Math/src/DataStructures/Graphs/Octree.h"
#include "engine/Math/src/DataStructures/PileOfContainers.h"
#include "engine/MiteVox/src/Playground/Entity.h"

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
		void emplace(Entity* entity);
		void emplace(safety::SafeArray<Entity*>* entitiesStack);
		void getAll(safety::SafeArray<Entity*>* entitiesStack);

		void getCollisions(safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions, mathem::PileOfSafeArrays<Entity*>* dataPointsContainers, float equalityTolerance);

		void update();
		
	private:

		mathem::Octree<Entity*>* octree = nullptr;
		size_t chunkLevel = 0;
		int32_t preferredLevel = 2; // TODO: add to settings
		size_t maxEmplacedDatapointsPerUpdate = 64;
		safety::SafeArray<Entity*> waitingDatapointsStack;
		safety::SafeArray<Entity*> datapointsToMove;
	};
}

#endif