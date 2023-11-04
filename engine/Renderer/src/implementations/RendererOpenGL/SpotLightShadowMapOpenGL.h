#ifndef SPOTLIGHTSHADOWMAPOPENGL_H
#define SPOTLIGHTSHADOWMAPOPENGL_H

#include "engine/Renderer/src/RendererAPI/Light.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace render
{
	class SpotLightShadowMapOpenGL
	{
	public:

		SpotLightShadowMapOpenGL();
		~SpotLightShadowMapOpenGL();

		void init(size_t width, size_t height, SpotLight spotLight);
		void activate(int shaderID);
		void passToLightingShader(int shaderID, size_t spotLightIndex);
		void clear();

		glm::mat4 getLightMatrix();

	private:

		size_t frameBufferObjectID = 0;
		size_t depthTextureID = 0;

		size_t width = 0;
		size_t height = 0;

		SpotLight spotLight;
		glm::mat4 lightMatrix;

		static glm::mat4 makeLightMatrix(SpotLight spotLight);
	};
}

#endif
