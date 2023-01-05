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
		octree = new mathem::Octree<Node>(halfSize, nodeMinHalfSize, nodeCapacity, nodeMaxLifeTime);
		this->chunkLevel = chunkLevel;
		this->maxEmplacedDatapointsPerUpdate = maxEmplacedDatapointsPerUpdate;
	}

	SceneFoundation::~SceneFoundation()
	{
		delete octree;
	}

	void SceneFoundation::emplace(Node* node)
	{
		waitingDatapointsStack.appendElement(node);
	}

	void SceneFoundation::emplace(safety::SafeArray<Node*>* nodesStack)
	{
		waitingDatapointsStack.concatenate(nodesStack);
	}

	void SceneFoundation::getAll(safety::SafeArray<Node*>* nodesStack)
	{
		octree->getAll(nodesStack);
	}

	void SceneFoundation::update()
	{
		size_t emplacedDatapointsCount = waitingDatapointsStack.getElementsCount();
		if (emplacedDatapointsCount > maxEmplacedDatapointsPerUpdate)
		{
			emplacedDatapointsCount = maxEmplacedDatapointsPerUpdate;
		}

		for (size_t i = 0; i < emplacedDatapointsCount; i++)
		{
			Node* datapoint = waitingDatapointsStack.getLastElement();
			waitingDatapointsStack.removeLastElement();
			if (datapoint->collider)
			{
				octree->emplace(datapoint);
			}
		}

		octree->update();
	}
}