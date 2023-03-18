#include "collectLights.h"

#include "engine/Renderer/src/RendererAPI/Light.h"

namespace mitevox
{
	void collectPointLightsRecursively(
		Node* node, 
		safety::SafeArray<render::PointLight>* resultPointLightsArray,
		safety::SafeArray<render::DirectionalLight>* resultDirectionalLightsArray,
		safety::SafeArray<render::SpotLight>* resultSpotLightsArray,
		mathem::GeometryTransform transform)
	{
		transform *= node->transform;

		switch (node->lightType)
		{
		case render::LightType::POINT:
			node->light.pointLight.position = transform.translation;
			resultPointLightsArray->appendElement(node->light.pointLight);
			break;

		case render::LightType::DIRECTIONAL:
			node->light.directionalLight.direction = transform.rotation;
			resultDirectionalLightsArray->appendElement(node->light.directionalLight);
			break;

		case render::LightType::SPOT:
			node->light.spotLight.position = transform.translation;
			node->light.spotLight.direction = transform.rotation;
			resultSpotLightsArray->appendElement(node->light.spotLight);
			break;

		default:
			break;
		}

		size_t subnodesCount = node->children.getElementsCount();
		for (size_t i = 0; i < subnodesCount; ++i)
		{
			collectPointLightsRecursively(
				node->children.getElement(i), 
				resultPointLightsArray, 
				resultDirectionalLightsArray, 
				resultSpotLightsArray,
				transform);
		}
	}

	void collectPointLights(
		safety::SafeArray<Entity*>* entities,
		safety::SafeArray<render::PointLight>* resultPointLightsArray,
		safety::SafeArray<render::DirectionalLight>* resultDirectionalLightsArray,
		safety::SafeArray<render::SpotLight>* resultSpotLightsArray)
	{
		size_t entitiesCount = entities->getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			Entity* entity = entities->getElement(i);
			collectPointLightsRecursively(
				entity->renderableNode,
				resultPointLightsArray,
				resultDirectionalLightsArray,
				resultSpotLightsArray,
				entity->transform);
		}
	}
}