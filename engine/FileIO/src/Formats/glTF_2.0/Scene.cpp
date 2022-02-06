#include "Scene.h"

namespace fileio
{
	Scene::Scene()
    {

    }

    void Scene::fromGLTF(JSON* sceneJSON, safety::SafeArray<Node*>* nodes)
    {
		_name = sceneJSON->getFieldString("name");

		JSON* nodesArrayJSON = sceneJSON->getFieldArray("nodes");
		if (nodesArrayJSON != nullptr)
		{
			size_t nodesCount = nodesArrayJSON->getArraySize();
			_nodes.resize(nodesCount);
			_nodes.fillWithZeros();

			for (size_t i = 0; i < nodesCount; ++i)
			{
				int32_t nodeIndex = (int32_t)nodesArrayJSON->getArrayItemNumber(i);
				_nodes.setElement(i, nodes->getElement(nodeIndex));
			}
		}
    }
}