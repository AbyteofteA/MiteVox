#include "SceneFoundation.h"

namespace mitevox
{
	SceneFoundation::SceneFoundation(
		float halfSize,
		float nodeMinHalfSize,
		size_t nodeCapacity,
		size_t nodeMaxLifeTime,
		size_t chunkLevel,
		size_t maxEmplacedDatapointsPerUpdate)
	{
		octree = new mathem::Octree<Entity*>(halfSize, nodeMinHalfSize, nodeCapacity, nodeMaxLifeTime);
		this->chunkLevel = chunkLevel;
		this->maxEmplacedDatapointsPerUpdate = maxEmplacedDatapointsPerUpdate;
		waitingDatapointsStack.reserve(64); // TODO: add to settings
		datapointsToMove.reserve(64); // TODO: add to settings
	}

	SceneFoundation::~SceneFoundation()
	{
		delete octree;
	}

	void SceneFoundation::emplace(Entity* entity)
	{
		waitingDatapointsStack.appendElement(entity);
	}

	void SceneFoundation::emplace(safety::SafeArray<Entity*>* entitiesStack)
	{
		waitingDatapointsStack.concatenate(entitiesStack);
	}

	void SceneFoundation::getAll(safety::SafeArray<Entity*>* entitiesStack)
	{
		octree->getAll(entitiesStack);
	}

	void SceneFoundation::getCollisions(safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions, mathem::PileOfSafeArrays<Entity*>* dataPointsContainers, float equalityTolerance)
	{
		octree->getCollisions(collisions, dataPointsContainers, equalityTolerance);
	}

	void SceneFoundation::update()
	{
		octree->update(preferredLevel, &datapointsToMove);
		waitingDatapointsStack.concatenate(&datapointsToMove);
		datapointsToMove.clear();

		size_t emplacedDatapointsCount = waitingDatapointsStack.getElementsCount();
		if (emplacedDatapointsCount > maxEmplacedDatapointsPerUpdate)
		{
			emplacedDatapointsCount = maxEmplacedDatapointsPerUpdate;
		}

		for (size_t i = 0; i < emplacedDatapointsCount; i++)
		{
			Entity* datapoint = waitingDatapointsStack.getLastElement();
			waitingDatapointsStack.removeLastElement();
			octree->emplaceAtLevel(datapoint, preferredLevel);
		}
	}
}