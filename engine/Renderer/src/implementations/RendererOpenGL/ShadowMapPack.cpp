#include "ShadowMapPack.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Renderer/src/RendererAPI/Cubemap.h"

#include <cassert>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef NDEBUG
#undef NDEBUG
#endif

namespace render
{
	ShadowMapPack::ShadowMapPack()
	{

	}

	ShadowMapPack::~ShadowMapPack()
	{
		clear();
	}

	void ShadowMapPack::init(size_t shadowsCount, size_t width, size_t height)
	{
		if (this->width == width &&
			this->height == height &&
			this->shadowsCapacity >= shadowsCount)
		{
			this->shadowsCount = shadowsCount;
			lightMatrices.resize(shadowsCount);
			return;
		}
		else
		{
			clear();
		}

		this->shadowsCount = shadowsCount;
		this->shadowsCapacity = shadowsCount;
		this->width = width;
		this->height = height;

		glGenTextures(1, (GLuint*)&depthTextureID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, (GLuint)depthTextureID);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		static const float textureBorderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, textureBorderColor);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, width, height, shadowsCount, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		glGenFramebuffers(1, (GLuint*)&frameBufferObjectID);
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)frameBufferObjectID);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, (GLuint)depthTextureID, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR: " << "Cannot create ShadowMapPack" << std::endl;
			assert(status != 0);
		}

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowMapPack::activate(int shaderID)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)frameBufferObjectID);
		setViewport(0, 0, width, height);
		shaders[shaderID]->setInt("shadowMapsCount", lightMatrices.getElementsCount());
		shaders[shaderID]->setMat4Array("lightMatrices", &lightMatrices);
	}

	void ShadowMapPack::passToLightingShader(int shaderID)
	{
		const size_t shadowMapTextureUnit = TEXTURE_UNIT_SPOT_SHADOWS_START;
		glActiveTexture(GL_TEXTURE0 + shadowMapTextureUnit);
		glBindTexture(GL_TEXTURE_2D_ARRAY, depthTextureID);
		shaders[shaderID]->setInt("shadowMapPack", shadowMapTextureUnit);
		shaders[shaderID]->setMat4Array("lightMatrices", &lightMatrices);
	}

	void ShadowMapPack::clear()
	{
		if (depthTextureID)
		{
			glDeleteTextures(1, (GLuint*)&depthTextureID);
		}
		if (frameBufferObjectID)
		{
			glDeleteFramebuffers(1, (GLuint*)&frameBufferObjectID);
		}
		frameBufferObjectID = 0;
		depthTextureID = 0;
		shadowsCount = 0;
		shadowsCapacity = 0;
		width = 0;
		height = 0;
	}

	void ShadowMapPack::makeLightMatrices(safety::SafeArray<SpotLight>* spotLightsArray, size_t offset, size_t count)
	{
		isCubemap = false;

		size_t remainingSpotLightsCount = spotLightsArray->getElementsCount() - offset;
		count = std::min(count, remainingSpotLightsCount);
		count = std::min(count, (size_t)SHADOW_MAP_PACK_SIZE);
		lightMatrices.resize(count);

		for (size_t i = 0; i < count; ++i)
		{
			lightMatrices[i] = makeLightMatrix(spotLightsArray->getElement(i + offset));
		}
	}

	void ShadowMapPack::makeLightMatrices(PointLight pointLight)
	{
		isCubemap = true;

		lightMatrices.resize(6);

		for (size_t i = 0; i < 6; ++i)
		{
			lightMatrices[i] = makeLightMatrix(pointLight, i);
		}
	}

	void ShadowMapPack::makeLightMatrices(DirectionalLight directionalLight, Camera* camera, mathem::GeometryTransform* cameraTransform, size_t cascadesCount)
	{
		isCubemap = false;

		cascadesCount = std::min(cascadesCount, (size_t)SHADOW_MAP_PACK_SIZE);
		lightMatrices.resize(cascadesCount);

		for (size_t i = 0; i < cascadesCount; ++i)
		{
			lightMatrices[i] = makeLightMatrix(directionalLight, camera, cameraTransform, i);
		}
	}

	glm::mat4 ShadowMapPack::getLightMatrix(size_t index)
	{
		return lightMatrices[index];
	}

	glm::mat4 ShadowMapPack::makeLightMatrix(SpotLight spotLight)
	{
		glm::mat4 viewMatrix = glm::lookAt(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(spotLight.direction.x(), spotLight.direction.y(), spotLight.direction.z()),
			glm::vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = glm::translate(viewMatrix, glm::vec3(-spotLight.position.x(), -spotLight.position.y(), -spotLight.position.z()));

		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		projectionMatrix = glm::perspective(spotLight.outerConeAngle * 2.0f, 1.0f, 0.001f, spotLight.lightBase.range);

		return projectionMatrix * viewMatrix;
	}

	glm::mat4 ShadowMapPack::makeLightMatrix(PointLight pointLight, size_t side)
	{
		glm::vec3 direction;
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		switch ((CubemapSides)side)
		{
		case CubemapSides::RIGHT:
			direction = glm::vec3(1.0f, 0.0f, 0.0f);
			break;

		case CubemapSides::LEFT:
			direction = glm::vec3(-1.0f, 0.0f, 0.0f);
			break;

		case CubemapSides::UP:
			direction = glm::vec3(0.0f, 1.0f, 0.0f);
			up = glm::vec3(0.0f, 0.0f, 1.0f);
			break;

		case CubemapSides::DOWN:
			direction = glm::vec3(0.0f, -1.0f, 0.0f);
			up = glm::vec3(0.0f, 0.0f, -1.0f);
			break;

		case CubemapSides::BACK:
			direction = glm::vec3(0.0f, 0.0f, 1.0f);
			break;

		case CubemapSides::FRONT:
			direction = glm::vec3(0.0f, 0.0f, -1.0f);
			break;
		}

		glm::vec3 pointLightPosition = glm::vec3(pointLight.position.x(), pointLight.position.y(), pointLight.position.z());
		glm::mat4 viewMatrix = glm::lookAt(pointLightPosition, pointLightPosition + direction, up);
		
		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, pointLight.lightBase.range);

		return projectionMatrix * viewMatrix;
	}

	glm::mat4 ShadowMapPack::makeLightMatrix(
		DirectionalLight directionalLight, 
		Camera* camera, 
		mathem::GeometryTransform* cameraTransform, 
		size_t cascade)
	{
		// TODO: 
		return glm::mat4();
	}
}