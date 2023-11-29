#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/NumericalAnalysis/Intertolation.h"
#include "engine/CodeSafety/src/SafeArray.h"

#ifdef NDEBUG
	#undef NDEBUG
#endif

#include <cassert>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace render
{
	int printFramebufferStatus()
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			std::cout << "Framebuffer complete" << std::endl;
			break;

		case GL_FRAMEBUFFER_UNDEFINED:
			std::cout << "ERROR: GL_FRAMEBUFFER_UNDEFINED " << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "ERROR: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT " << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cout << "ERROR: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT " << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cout << "ERROR: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER " << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cout << "ERROR: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER " << std::endl;
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "ERROR: GL_FRAMEBUFFER_UNSUPPORTED " << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			std::cout << "ERROR: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE " << std::endl;
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			std::cout << "ERROR: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS " << std::endl;
			break;

		case 0:
		default:
			std::cout << "ERROR: Unknown framebuffer error" << std::endl;
			break;
		}

		return status;
	}

	void renderScreenQuad()
	{
		glBindVertexArray(getScreenQuadID());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void activateDefaultFramebuffer(RendererSettings* renderer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)0);
		glViewport(0, 0, renderer->screenWidth, renderer->screenHeight);
	}

	void clearBufferXY(ColorRGBf color)
	{
		glClearColor(color.r, color.g, color.b, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void clearBufferZ()
	{
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void display(RendererSettings* renderer)
	{
		glfwSwapBuffers(renderer->getWindow());
	}

	size_t gBufferID = 0;
	size_t positionTextureID = 0;
	size_t normalTextureID = 0;
	size_t albedoTextureID = 0;
	size_t materialTextureID = 0;
	size_t depthTextureID = 0;

	void createGbuffer(RendererSettings* renderer)
	{
		std::cout << "Creating " << renderer->screenWidth  << "x" << renderer->screenHeight << " G-Buffer" << std::endl;

		// Position texture
		glGenTextures(1, &positionTextureID);
		glBindTexture(GL_TEXTURE_2D, positionTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, renderer->screenWidth, renderer->screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Normal texture
		glGenTextures(1, &normalTextureID);
		glBindTexture(GL_TEXTURE_2D, normalTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, renderer->screenWidth, renderer->screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Albedo texture
		glGenTextures(1, &albedoTextureID);
		glBindTexture(GL_TEXTURE_2D, albedoTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderer->screenWidth, renderer->screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Material texture
		glGenTextures(1, &materialTextureID);
		glBindTexture(GL_TEXTURE_2D, materialTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderer->screenWidth, renderer->screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Depth texture
		glGenTextures(1, &depthTextureID);
		glBindTexture(GL_TEXTURE_2D, depthTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, renderer->screenWidth, renderer->screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenFramebuffers(1, &gBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, gBufferID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTextureID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTextureID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedoTextureID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, materialTextureID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);
		unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);

		GLenum status = printFramebufferStatus();
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR: " << "Cannot create G-Buffer" << std::endl;
			assert(status == GL_FRAMEBUFFER_COMPLETE);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void activateGbuffer(RendererSettings* renderer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)gBufferID);
		setViewport(0, 0, renderer->screenWidth, renderer->screenHeight);
	}

	void deleteGbuffer()
	{
		if (positionTextureID)
		{
			glDeleteTextures(1, (GLuint*)&positionTextureID);
		}
		if (normalTextureID)
		{
			glDeleteTextures(1, (GLuint*)&normalTextureID);
		}
		if (albedoTextureID)
		{
			glDeleteTextures(1, (GLuint*)&albedoTextureID);
		}
		if (materialTextureID)
		{
			glDeleteTextures(1, (GLuint*)&materialTextureID);
		}
		if (depthTextureID)
		{
			glDeleteTextures(1, (GLuint*)&depthTextureID);
		}
		if (gBufferID)
		{
			glDeleteFramebuffers(1, (GLuint*)&gBufferID);
		}
	}

	size_t mainCanvasBufferID = 0;
	size_t mainCanvasTextureID = 0;
	size_t mainCanvasDepthRenderbufferID = 0;

	void createMainCanvas(RendererSettings* renderer)
	{
		glGenTextures(1, &mainCanvasTextureID);
		glBindTexture(GL_TEXTURE_2D, mainCanvasTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, renderer->screenWidth, renderer->screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenRenderbuffers(1, &mainCanvasDepthRenderbufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, mainCanvasDepthRenderbufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, renderer->screenWidth, renderer->screenHeight);

		glGenFramebuffers(1, &mainCanvasBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, mainCanvasBufferID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mainCanvasTextureID, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mainCanvasDepthRenderbufferID);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR: " << "Cannot create main canvas" << std::endl;
			assert(status == GL_FRAMEBUFFER_COMPLETE);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void activateMainCanvas(RendererSettings* renderer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)mainCanvasBufferID);
		setViewport(0, 0, renderer->screenWidth, renderer->screenHeight);
	}

	void renderSceneFromGbuffer(
		RendererSettings* renderer,
		int shaderID,
		Camera* camera,
		mathem::GeometryTransform* cameraTransform,
		float deltaTime)
	{
		shaders[shaderID]->setFloat("deltaTime", deltaTime);
		shaders[shaderID]->setCamera(camera);
		shaders[shaderID]->setVec3("viewPos",
			cameraTransform->translation.x(),
			cameraTransform->translation.y(),
			cameraTransform->translation.z());
		shaders[shaderID]->setCameraMatrices(camera);
		shaders[shaderID]->setCameraInverseMatrices(camera);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, positionTextureID);
		shaders[shaderID]->setInt("positionTexture", 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, normalTextureID);
		shaders[shaderID]->setInt("normalTexture", 1);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, albedoTextureID);
		shaders[shaderID]->setInt("albedoTexture", 2);

		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, materialTextureID);
		shaders[shaderID]->setInt("materialTexture", 3);

		renderScreenQuad();
	}

	void copyDepthFromGbufferToMainCanvas(RendererSettings* renderer)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBufferID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mainCanvasBufferID);
		glBlitFramebuffer(
			0,
			0,
			renderer->screenWidth,
			renderer->screenHeight,
			0,
			0,
			renderer->screenWidth,
			renderer->screenHeight,
			GL_DEPTH_BUFFER_BIT,
			GL_NEAREST);
	}

	void renderSceneFromMainCanvas(RendererSettings* renderer, int shaderID, float deltaTime)
	{
		render::useShader(shaderID);

		shaders[shaderID]->setFloat("deltaTime", deltaTime);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, mainCanvasTextureID);
		shaders[shaderID]->setInt("mainCanvasTexture", 0);

		renderScreenQuad();
	}

	void deleteMainCanvas()
	{
		if (mainCanvasTextureID)
		{
			glDeleteTextures(1, (GLuint*)&mainCanvasTextureID);
		}
		if (mainCanvasBufferID)
		{
			glDeleteFramebuffers(1, (GLuint*)&mainCanvasBufferID);
		}
	}
}
