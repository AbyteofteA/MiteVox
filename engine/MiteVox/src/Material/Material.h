
#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "engine/Renderer/src/RendererAPI/Color.h"
#include "engine/MiteVox/src/Material/Image.h"
#include "engine/MiteVox/src/Material/Texture.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace mitevox
{
	class Material
	{
	public:

		std::string name;
		bool isDoubleSided = false;

		// Color and shading
		render::ColorRGBAf baseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		float roughness = 1.0f;
		float metallicity = 1.0f;
		render::ColorRGBf emission = { 0.0f, 0.0f, 0.0f };	// (0.0 - 1.0)
		float specularExponent = 8;					// (0.0 - 1000.0)

		// Transparancy
		render::ColorRGBAf filter = { 1.0f, 1.0f, 1.0f };
		float transparency = 0;						// (0.0 - 1.0)
		float opticalDensity;						// (0.001 - 10.0)
		float reflectionSharpness;					// (0.0 - 1000.0)
													
		int illuminationModel = 2;						// (0 - 10)

		std::string alphaMode = "OPAQUE";
		float alphaCutoff = 0.5f;

		mitevox::Texture* albedoMap = nullptr;
		mitevox::Texture* metallicRoughnessMap = nullptr;
		mitevox::Texture* normalMap = nullptr;
		mitevox::Texture* occlusionMap = nullptr;
		mitevox::Texture* emissiveMap = nullptr;
	};
}

#endif