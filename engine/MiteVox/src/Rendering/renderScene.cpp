#include "engine/MiteVox/src/MiteVoxAPI.h"

#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/MiteVox/src/Rendering/drawLightSource.h"
#include "engine/Renderer/src/RendererAPI/Color.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"

namespace mitevox
{
	void renderNodeRecursively(
		render::RendererSettings* renderer,
		int shaderID,
		Node* node,
		mathem::GeometryTransform* nodeTransform,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform,
		glm::mat4 viewProjectionMatrix);

	void MiteVoxAPI::renderScene(
		render::RendererSettings* renderer,
		int shadowMapShaderID,
		int lightingShaderID,
		mathem::Vector3D ambientLight,
		safety::SafeArray<render::PointLight>* pointLightsArray,
		safety::SafeArray<render::DirectionalLight>* directionalLightsArray,
		safety::SafeArray<render::SpotLight>* spotLightsArray,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform,
		glm::mat4 viewProjectionMatrix,
		safety::SafeArray<Entity*> entities,
		int skyboxShaderID,
		render::Cubemap* skybox)
	{
		if (skybox)
		{
			render::renderSkybox(renderer, skyboxShaderID, skybox, camera, cameraTransform);
		}
		
		render::useShader(lightingShaderID);
		
		render::resetLights(lightingShaderID);
		render::setAmbientLight(ambientLight, lightingShaderID);
		render::uploadDirectionalLights(directionalLightsArray, lightingShaderID);
		render::uploadPointLights(pointLightsArray, lightingShaderID);
		
		glDepthFunc(GL_LEQUAL);
		
		// TODO: replace with G-Buffer rendering
		size_t entitiesCount = entities.getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			Entity* entity = entities.getElement(i);
			renderNodeRecursively(
				renderer,
				lightingShaderID,
				entity->renderableNode,
				&entity->transform,
				camera,
				cameraTransform,
				viewProjectionMatrix);
		}

		renderSceneWithSpotLights(renderer, shadowMapShaderID, lightingShaderID, spotLightsArray, entities, camera, cameraTransform, viewProjectionMatrix);
	}

	void renderNodeRecursively(
		render::RendererSettings* renderer,
		int shaderID,
		Node* node,
		mathem::GeometryTransform* nodeTransform,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform,
		glm::mat4 viewProjectionMatrix)
	{
		mathem::GeometryTransform nodeGlobalTransform = *nodeTransform * node->transform;

		if (node->lightType != render::LightType::NONE)
		{
			drawLightSource(renderer, node, nodeTransform);
		}

		if (mitevox::Mesh* meshToRender = node->getMeshToRender())
		{
			render::tryUploadSkeleton(node, shaderID);
			if (meshToRender->isUploaded == false)
			{
				render::uploadMesh(meshToRender, shaderID);
			}
			render::renderMesh(renderer, shaderID, meshToRender, &nodeGlobalTransform, camera, cameraTransform, viewProjectionMatrix);
		}

		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			renderNodeRecursively(
				renderer,
				shaderID,
				node->children.getElement(i),
				&nodeGlobalTransform,
				camera,
				cameraTransform, 
				viewProjectionMatrix);
		}
	}

	void renderNodeToSpotLightShadowMapRecursively(
		render::RendererSettings* renderer,
		int shaderID,
		Node* node,
		mathem::GeometryTransform* nodeTransform)
	{
		mathem::GeometryTransform nodeGlobalTransform = *nodeTransform * node->transform;

		if (mitevox::Mesh* meshToRender = node->getMeshToRender())
		{
			render::tryUploadSkeleton(node, shaderID);
			if (meshToRender->isUploaded == false)
			{
				render::uploadMesh(meshToRender, shaderID);
			}
			render::renderMeshToSpotLightShadowMap(renderer, shaderID, meshToRender, &nodeGlobalTransform);
		}

		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			renderNodeToSpotLightShadowMapRecursively(
				renderer,
				shaderID,
				node->children.getElement(i),
				&nodeGlobalTransform);
		}
	}

	void renderSceneToSpotLightShadowMap(
		render::RendererSettings* renderer,
		int shaderID,
		safety::SafeArray<Entity*> entities)
	{
		render::useShader(shaderID);

		size_t entitiesCount = entities.getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			Entity* entity = entities.getElement(i);
			renderNodeToSpotLightShadowMapRecursively(
				renderer,
				shaderID,
				entity->renderableNode,
				&entity->transform);
		}
	}

	void MiteVoxAPI::renderSceneWithSpotLights(
		render::RendererSettings* renderer,
		int shadowMapShaderID,
		int lightingShaderID,
		safety::SafeArray<render::SpotLight>* spotLightsArray,
		safety::SafeArray<Entity*> entities,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform,
		glm::mat4 viewProjectionMatrix)
	{
		size_t spotLightsCount = spotLightsArray->getElementsCount();
		size_t spotLightsPerFrame = std::min(spotLightsCount, renderer->spotLightsPerFrame);

		size_t spotLightOffset = 0;
		while (spotLightOffset < spotLightsPerFrame)
		{
			// Render shadow maps

			render::useShader(shadowMapShaderID);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glDepthFunc(GL_LEQUAL);

			size_t spotLightsPerCall = std::min(spotLightsCount - spotLightOffset, renderer->spotLightsPerCall);
			render::tryAllocateSpotLightShadowMaps(spotLightsPerCall);

			for (size_t i = 0; i < spotLightsPerCall; ++i)
			{
				render::SpotLight spotLight = spotLightsArray->getElement(i + spotLightOffset);
				render::SpotLightShadowMapOpenGL& spotLightShadowMap = render::spotLightShadowMaps[i];
				spotLightShadowMap.init(renderer->spotLightShadowMapSize, renderer->spotLightShadowMapSize, spotLight);
				render::selectSpotLightShadowMap(shadowMapShaderID, i);
				render::clearBufferZ();
				renderSceneToSpotLightShadowMap(renderer, shadowMapShaderID, entities);
			}

			// Render the scene

			render::activateDefaultFramebuffer(renderer->screenWidth, renderer->screenHeight);
			render::useShader(lightingShaderID);
			render::uploadSpotLights(spotLightsPerCall, lightingShaderID);

			glCullFace(GL_BACK);
			if (renderer->backfaceCulling)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
			glDepthFunc(GL_EQUAL);

			size_t entitiesCount = entities.getElementsCount();
			for (size_t i = 0; i < entitiesCount; ++i)
			{
				Entity* entity = entities.getElement(i);
				renderNodeRecursively(
					renderer,
					lightingShaderID,
					entity->renderableNode,
					&entity->transform,
					camera,
					cameraTransform,
					viewProjectionMatrix);
			}

			spotLightOffset += spotLightsPerCall;
		}
	}
}
