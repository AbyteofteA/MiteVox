#include "Scene.h"

namespace fileio
{
	Scene::Scene()
    {

    }

    void Scene::fromGLTF(JSON* sceneJSON)
    {
		name = sceneJSON->getFieldString("name");

		JSON* nodesArrayJSON = sceneJSON->getFieldArray("nodes");
		if (nodesArrayJSON != nullptr)
		{
			nodesArrayJSON->toNumberArray<int32_t>(&nodes);
		}
    }
}