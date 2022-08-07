#include "SceneCodec.h"

#include "engine/MiteVox/src/Playground/Scene.h"

namespace fileio
{
    void SceneCodec::fromGLTF(
		mitevox::Scene* sceneResult,
		JSON* sceneJSON, 
		safety::SafeArray<mitevox::Node*>* nodes)
    {
		sceneResult->name = sceneJSON->getFieldString("name");

		JSON* nodesArrayJSON = sceneJSON->getFieldArray("nodes");
		if (nodesArrayJSON != nullptr)
		{
			size_t nodesCount = nodesArrayJSON->getArraySize();
			sceneResult->nodes.resize(nodesCount);
			sceneResult->nodes.fillWithZeros();

			for (size_t i = 0; i < nodesCount; ++i)
			{
				int32_t nodeIndex = (int32_t)nodesArrayJSON->getArrayItemNumber(i);
				sceneResult->nodes.setElement(i, nodes->getElement(nodeIndex));
			}
		}
    }
}