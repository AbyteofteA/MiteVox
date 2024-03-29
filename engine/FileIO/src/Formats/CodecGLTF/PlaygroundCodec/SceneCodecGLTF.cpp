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
			sceneResult->entities.setAllElementsZeros();

			for (size_t entityIndex = 0; entityIndex < entitiesCount; ++entityIndex)
			{
				int32_t nodeIndex = (int32_t)nodesArrayJSON->getArrayItemNumberOrDefault(entityIndex, -1.0f);
				mitevox::Node* renderableNode = nodes->getElement(nodeIndex);
				prepareNodeRecursively(renderableNode);

				// TODO: move to preparePlayground()
				mitevox::Entity* entity = new mitevox::Entity();
				entity->renderableNode = renderableNode;
				sceneResult->entities.setElement(entityIndex, entity);
			}
		}
    }

	void SceneCodec::toGLTF(
		JSON* sceneJSONResult,
		mitevox::Scene* scene,
		safety::SafeArray<mitevox::Node*>* nodes)
	{
		sceneJSONResult->setField("name", scene->name);

		size_t nodesCount = nodes->getElementsCount();
		size_t entitiesCount = scene->entities.getElementsCount();
		if (entitiesCount > 0)
		{
			JSON* nodesArrayJSON = sceneJSONResult->setFieldType("nodes", JSONtype::ARRAY);
			
			for (size_t entityIndex = 0; entityIndex < entitiesCount; ++entityIndex)
			{
				for (size_t nodeIndex = 0; nodeIndex < nodesCount; ++nodeIndex)
				{
					mitevox::Entity* entity = scene->entities.getElement(entityIndex);
					if (entity->renderableNode == nodes->getElement(nodeIndex))
					{
						nodesArrayJSON->addArrayItem((double)nodeIndex);
						break;
					}
				}
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