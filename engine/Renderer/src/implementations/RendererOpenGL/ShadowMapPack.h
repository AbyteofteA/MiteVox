#ifndef SHADOWMAPPACK_H
#define SHADOWMAPPACK_H

#include "engine/Renderer/src/RendererAPI/Light.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define SHADOW_MAP_PACK_SIZE 16

namespace render
{
	class ShadowMapPack
	{
	public:

		ShadowMapPack();
		~ShadowMapPack();

		void init(size_t shadowsCount, size_t width, size_t height);
		void activate(int shaderID);
		void passToLightingShader(int shaderID);
		void clear();

		void makeLightMatrices(safety::SafeArray<SpotLight>* spotLightsArray, size_t offset, size_t count);
		void makeLightMatrices(PointLight pointLight);
		void makeLightMatrices(DirectionalLight directionalLight, Camera* camera, mathem::GeometryTransform* cameraTransform, size_t cascadesCount);
		glm::mat4 getLightMatrix(size_t index);

	private:

		bool isCubemap = false;
		size_t frameBufferObjectID = 0;
		size_t depthTextureID = 0;

		size_t shadowsCount = 0;
		size_t shadowsCapacity = 0;
		size_t width = 0;
		size_t height = 0;

		safety::SafeArray<glm::mat4> lightMatrices;

		static glm::mat4 makeLightMatrix(SpotLight spotLight);
		static glm::mat4 makeLightMatrix(PointLight pointLight, size_t side);
		static glm::mat4 makeLightMatrix(
			DirectionalLight directionalLight, 
			Camera* camera, 
			mathem::GeometryTransform* cameraTransform, 
			size_t cascade);
	};
}

#endif

