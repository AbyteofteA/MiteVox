
#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "engine/Renderer/src/RendererAPI/Color.h"
#include "engine/MiteVox/src/Material/Image.h"
#include "engine/MiteVox/src/Material/Texture.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace mitevox
{
	enum struct IlluminationModel
	{
		NONE = 0,
		UNLIT = 1,
		PHONG = 2,
		PBR = 32
	};

	class Material
	{
	public:

		std::string name;
		bool isDoubleSided = false;

		// Color and shading
		render::ColorRGBAf baseColor = render::ColorRGBAf(render::ColorRGBf::WHITE());
		float roughness = 1.0f;
		float metallicity = 0.0f;
		render::ColorRGBf emission = render::ColorRGBf::BLACK();	// (0.0f - 1.0f)
		float specularExponent = 8;					// (0.0f - 1000.0f)
							
		IlluminationModel illuminationModel = IlluminationModel::PBR;	// (0 - 10)

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