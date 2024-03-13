#include "engine/MiteVox/src/MiteVoxAPI.h"

#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/MiteVox/src/Rendering/drawCollider.h"
#include "engine/MiteVox/src/Rendering/drawCollisions.h"
#include "engine/MiteVox/src/Rendering/drawSceneOctree.h"
#include "engine/MiteVox/src/Rendering/drawAxes.h"
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
		mathem::GeometryTransform* cameraTransform);

	void MiteVoxAPI::renderScene(
		render::RendererSettings* renderer,
		int shadowMapShaderID,
		int gBufferShaderID,
		int deferredLightingShaderID,
		int postprocessingShaderID,
		mathem::Vector3D ambientLight,
		safety::SafeArray<render::PointLight>* pointLightsArray,
		safety::SafeArray<render::DirectionalLight>* directionalLightsArray,
		safety::SafeArray<render::SpotLight>* spotLightsArray,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform,
		safety::SafeArray<Entity*> entities,
		bool debug,
		int skyboxShaderID,
		render::Cubemap* skybox)
	{
		if (debug)
		{
			render::setWireframeRendering(true);
		}

		// Render geometry to G-buffer
		render::activateGbuffer(renderer);
		render::clearBufferXY();
		render::clearBufferZ();
		renderSceneToGbuffer(renderer, gBufferShaderID, camera, cameraTransform, entities);
		render::copyDepthFromGbufferToMainCanvas(renderer);

		render::setWireframeRendering(false);

		// Render lighting to main canvas
		render::activateMainCanvas(renderer);
		render::clearBufferXY();
		renderSceneWithSpotLights(
			renderer, shadowMapShaderID, deferredLightingShaderID, spotLightsArray, entities, camera, cameraTransform);
		renderSceneWithPointLights(
			renderer, shadowMapShaderID, deferredLightingShaderID, pointLightsArray, entities, camera, cameraTransform);

		// TODO: 
		if (debug)
		{
			drawAxes(renderer);
			//drawCollisions(renderer, &collisions);
		}

		render::setAdditiveBlending();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE);

		// Render primitives to main canvas
		render::renderPoints(renderer, camera, cameraTransform);
		render::renderLines(renderer, camera, cameraTransform);
		render::renderTriangles(renderer, camera, cameraTransform);

		glDepthMask(GL_FALSE);

		// Render main canvas to default framebuffer
		render::disableBlending();
		render::activateDefaultFramebuffer(renderer);
		render::clearBufferXY();
		render::clearBufferZ();
		render::renderSceneFromMainCanvas(renderer, postprocessingShaderID, MiteVoxAPI::getCurrentDeltaTime());
	}

	void MiteVoxAPI::renderSceneToGbuffer(
		render::RendererSettings* renderer,
		int shaderID,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform,
		safety::SafeArray<Entity*> entities)
	{
		render::useShader(shaderID);

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		render::disableBlending();

		size_t entitiesCount = entities.getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			Entity* entity = entities.getElement(i);
			renderNodeRecursively(
				renderer,
				shaderID,
				entity->renderableNode,
				entity->getResultTransform(),
				camera,
				cameraTransform);
		}
	}

	void renderNodeRecursively(
		render::RendererSettings* renderer,
		int shaderID,
		Node* node,
		mathem::GeometryTransform* nodeTransform,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform)
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
				render::uploadMesh(meshToRender);
			}
			render::renderMesh(renderer, shaderID, meshToRender, &nodeGlobalTransform, camera, cameraTransform);
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
				cameraTransform);
		}
	}

	void renderNodeToShadowMapRecursively(
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
				render::uploadMesh(meshToRender);
			}
			render::renderMeshToShadowMap(renderer, shaderID, meshToRender, &nodeGlobalTransform);
		}

		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			renderNodeToShadowMapRecursively(
				renderer,
				shaderID,
				node->children.getElement(i),
				&nodeGlobalTransform);
		}
	}

	void renderSceneToShadowMap(
		render::RendererSettings* renderer,
		int shaderID,
		safety::SafeArray<Entity*> entities)
	{
		render::useShader(shaderID);

		size_t entitiesCount = entities.getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			Entity* entity = entities.getElement(i);
			renderNodeToShadowMapRecursively(
				renderer,
				shaderID,
				entity->renderableNode,
				entity->getResultTransform());
		}
	}

	void MiteVoxAPI::renderSceneWithSpotLights(
		render::RendererSettings* renderer,
		int shadowMapShaderID,
		int lightingShaderID,
		safety::SafeArray<render::SpotLight>* spotLightsArray,
		safety::SafeArray<Entity*> entities,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform)
	{
		size_t spotLightsCount = spotLightsArray->getElementsCount();
		size_t spotLightsPerFrame = std::min(spotLightsCount, renderer->spotLightsPerFrame);

		size_t spotLightOffset = 0;
		while (spotLightOffset < spotLightsPerFrame)
		{
			// Render shadow maps

			render::useShader(shadowMapShaderID);
			render::disableBlending();
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);

			size_t spotLightsPerCall = std::min(spotLightsCount - spotLightOffset, renderer->spotLightsPerCall);
			render::shadowMapPack.init(spotLightsPerCall, renderer->spotLightShadowMapSize, renderer->spotLightShadowMapSize);
			render::shadowMapPack.makeLightMatrices(spotLightsArray, spotLightOffset, spotLightsPerCall);
			render::shadowMapPack.activate(shadowMapShaderID);
			render::clearBufferZ();
			renderSceneToShadowMap(renderer, shadowMapShaderID, entities);

			// Render the scene

			render::activateMainCanvas(renderer);
			render::useShader(lightingShaderID);
			render::resetLights(lightingShaderID);
			render::uploadSpotLights(spotLightsArray, spotLightOffset, spotLightsPerCall, lightingShaderID);
			render::shadowMapPack.passToLightingShader(lightingShaderID);

			glCullFace(GL_BACK);
			if (renderer->backfaceCulling)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
			render::setAdditiveBlending();
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);

			renderSceneFromGbuffer(
				renderer,
				lightingShaderID,
				camera,
				cameraTransform,
				MiteVoxAPI::getCurrentDeltaTime());

			spotLightOffset += spotLightsPerCall;
		}
	}

	void MiteVoxAPI::renderSceneWithPointLights(
		render::RendererSettings* renderer,
		int shadowMapShaderID,
		int lightingShaderID,
		safety::SafeArray<render::PointLight>* pointLightsArray,
		safety::SafeArray<Entity*> entities,
		render::Camera* camera,
		mathem::GeometryTransform* cameraTransform)
	{
		size_t lightsCount = pointLightsArray->getElementsCount();

		for (size_t i = 0; i < lightsCount; ++i)
		{
			render::PointLight pointLight = pointLightsArray->getElement(i);

			// Render shadow maps

			render::useShader(shadowMapShaderID);
			render::disableBlending();
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);

			render::shadowMapPack.init(6, renderer->pointLightShadowMapSize, renderer->pointLightShadowMapSize);
			render::shadowMapPack.makeLightMatrices(pointLight);
			render::shadowMapPack.activate(shadowMapShaderID);
			render::clearBufferZ();
			renderSceneToShadowMap(renderer, shadowMapShaderID, entities);

			// Render the scene

			render::activateMainCanvas(renderer);
			render::useShader(lightingShaderID);
			render::resetLights(lightingShaderID);
			render::uploadPointLights(pointLightsArray, i, 1, lightingShaderID);
			render::shadowMapPack.passToLightingShader(lightingShaderID);

			glCullFace(GL_BACK);
			if (renderer->backfaceCulling)
			{
				glEnable(GL_CULL_FACE);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
			render::setAdditiveBlending();
			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);

			renderSceneFromGbuffer(
				renderer,
				lightingShaderID,
				camera,
				cameraTransform,
				MiteVoxAPI::getCurrentDeltaTime());
		}
	}
}
