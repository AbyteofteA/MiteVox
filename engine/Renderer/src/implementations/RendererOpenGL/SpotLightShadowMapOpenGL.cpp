#include "SpotLightShadowMapOpenGL.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

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
	SpotLightShadowMapOpenGL::SpotLightShadowMapOpenGL()
	{

	}

	SpotLightShadowMapOpenGL::~SpotLightShadowMapOpenGL()
	{
		clear();
	}

	void SpotLightShadowMapOpenGL::init(size_t width, size_t height, SpotLight spotLight)
	{
		this->spotLight = spotLight;
		this->lightMatrix = SpotLightShadowMapOpenGL::makeLightMatrix(spotLight);

		if (this->width == width &&
			this->height == height)
		{
			return;
		}
		else
		{
			clear();
		}

		this->width = width;
		this->height = height;

		glGenTextures(1, (GLuint*)&depthTextureID);
		glBindTexture(GL_TEXTURE_2D, (GLuint)depthTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//static const float textureBorderColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//glSamplerParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureBorderColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, (GLuint*)&frameBufferObjectID);
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)frameBufferObjectID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, (GLuint)depthTextureID, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR: " << "Cannot create SpotLightShadowMapOpenGL" << std::endl;
			assert(status != 0);
		}

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void SpotLightShadowMapOpenGL::activate(int shaderID)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)frameBufferObjectID);
		setViewport(0, 0, width, height);
		shaders[shaderID]->setMat4("viewProjectionMatrix", lightMatrix);
	}

	void SpotLightShadowMapOpenGL::passToLightingShader(int shaderID, size_t spotLightIndex)
	{
		std::string indexStr = std::to_string(spotLightIndex);

		// Spot light

		static const std::string spotLights = "spotLights[";
		static const std::string pos = "].pos";
		static const std::string direction = "].direction";
		static const std::string innerConeAngle = "].innerConeAngle";
		static const std::string outerConeAngle = "].outerConeAngle";
		static const std::string color = "].color";
		static const std::string intensity = "].intensity";
		static const std::string range = "].range";
		std::string posResult = spotLights + indexStr + pos;
		std::string directionResult = spotLights + indexStr + direction;
		std::string innerConeAngleResult = spotLights + indexStr + innerConeAngle;
		std::string outerConeAngleResult = spotLights + indexStr + outerConeAngle;
		std::string colorResult = spotLights + indexStr + color;
		std::string intensityResult = spotLights + indexStr + intensity;
		std::string rangeResult = spotLights + indexStr + range;

		shaders[shaderID]->setVec3(
			posResult.c_str(),
			spotLight.position.x(),
			spotLight.position.y(),
			spotLight.position.z());
		shaders[shaderID]->setVec3(
			directionResult.c_str(),
			spotLight.direction.x(),
			spotLight.direction.y(),
			spotLight.direction.z());

		shaders[shaderID]->setFloat(innerConeAngleResult.c_str(), spotLight.innerConeAngle);
		shaders[shaderID]->setFloat(outerConeAngleResult.c_str(), spotLight.outerConeAngle);

		shaders[shaderID]->setVec3(
			colorResult.c_str(),
			spotLight.lightBase.color.r,
			spotLight.lightBase.color.g,
			spotLight.lightBase.color.b);

		shaders[shaderID]->setFloat(intensityResult.c_str(), spotLight.lightBase.intensity);
		shaders[shaderID]->setFloat(rangeResult.c_str(), spotLight.lightBase.range);

		// Spot light shadow map

		const size_t shadowMapTextureUnit = TEXTURE_UNIT_SPOT_SHADOWS_START + spotLightIndex;
		glActiveTexture(GL_TEXTURE0 + shadowMapTextureUnit);
		glBindTexture(GL_TEXTURE_2D, depthTextureID);
		
		static const std::string spotShadowMaps = "spotShadowMaps[";
		static const std::string spotLightMatrices = "spotLightMatrices[";
		std::string spotShadowMapsElement = spotShadowMaps + indexStr + "]";
		std::string spotLightMatricesElement = spotLightMatrices + indexStr + "]";
		
		shaders[shaderID]->setInt(spotShadowMapsElement.c_str(), shadowMapTextureUnit);
		shaders[shaderID]->setMat4(spotLightMatricesElement.c_str(), lightMatrix);

		PRINT_RENDERER_ERRORS;
	}

	void SpotLightShadowMapOpenGL::clear()
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
		width = 0;
		height = 0;
	}

	glm::mat4 SpotLightShadowMapOpenGL::getLightMatrix()
	{
		return lightMatrix;
	}

	glm::mat4 SpotLightShadowMapOpenGL::makeLightMatrix(SpotLight spotLight)
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
}