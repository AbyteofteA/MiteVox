#include "SceneCodec.h"

#include "engine/MiteVox/src/Playground/Scene.h"

namespace fileio
{
	void prepareNodeRecursively(mitevox::Node* node);

    void SceneCodec::fromGLTF(
		mitevox::Scene* sceneResult,
		JSON* sceneJSON, 
		safety::SafeArray<mitevox::Node*>* nodes)
    {
		sceneResult->name = sceneJSON->getFieldStringOrDefault("name", "Untitled");

		if (JSON* nodesArrayJSON = sceneJSON->getFieldArray("nodes"))
		{
			size_t entitiesCount = nodesArrayJSON->getArraySize();
			sceneResult->entities.resize(entitiesCount);
			sceneResult->entities.fillWithZeros();

			for (size_t i = 0; i < entitiesCount; ++i)
			{
				int32_t nodeIndex = (int32_t)nodesArrayJSON->getArrayItemNumberOrDefault(i, -1.0);
				mitevox::Node* renderableNode = nodes->getElement(nodeIndex);
				prepareNodeRecursively(renderableNode);

				mitevox::Entity* entity = new mitevox::Entity();
				entity->renderableNode = *renderableNode;
				sceneResult->entities.setElement(i, entity);
			}
		}
    }

	void prepareNodeRecursively(mitevox::Node* node)
	{
		if (node->mesh)
		{
			if (node->isMorphableMesh())
			{
				node->morphAnimationTarget = new mitevox::Mesh();
				node->mesh->makeCopyForAnimationTo(node->morphAnimationTarget);
			}

			// Apply default material where needed
			size_t meshPrimitivesCount = node->mesh->primitives.getElementsCount();
			for (size_t i = 0; i < meshPrimitivesCount; ++i)
			{
				mitevox::MeshPrimitive* meshPrimitive = node->mesh->primitives.getElement(i);
				if (meshPrimitive->material == nullptr)
				{
					meshPrimitive->material = new mitevox::Material();
				}
			}
		}
		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			prepareNodeRecursively(node->children.getElement(i));
		}
	}
}