#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

#include <cassert>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace render
{
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, renderer->screenWidth, renderer->screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
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

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR: " << "Cannot create G-Buffer" << std::endl;
			assert(status != 0);
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
			assert(status != 0);
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
		mathem::GeometryTransform* cameraTransform)
	{
		shaders[shaderID]->setVec3("viewPos",
			cameraTransform->translation.x(),
			cameraTransform->translation.y(),
			cameraTransform->translation.z());

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

		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, depthTextureID);
		shaders[shaderID]->setInt("depthTexture", 4);

		drawScreenQuad();
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

	void renderSceneFromMainCanvas(RendererSettings* renderer, int shaderID)
	{
		render::useShader(shaderID);

		// TODO: Set Parameters

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, mainCanvasTextureID);
		shaders[shaderID]->setInt("mainCanvasTexture", 0);

		drawScreenQuad();
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
