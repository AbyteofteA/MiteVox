
#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "engine/Renderer/src/RendererAPI/Color.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Material/Image.h"
#include "engine/FileIO/src/Formats/glTF_2.0/Material/Texture.h"
#include "engine/FileIO/src/FileStatus.h"
#include "engine/FileIO/src/FileInputOutput.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace fileio
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

		fileio::Texture* albedoMap = nullptr;
		fileio::Texture* metallicRoughnessMap = nullptr;
		fileio::Texture* normalMap = nullptr;
		fileio::Texture* occlusionMap = nullptr;
		fileio::Texture* emissiveMap = nullptr;

		void fromGLTF(JSON* materialJSON, safety::SafeArray<Texture*>* textures);
	};


	inline void loadMaterial_MTL(std::string filename, void** material, std::atomic<fileio::FileStatus>* flag)
	{
		*material = nullptr;
		flag->store(fileio::FileStatus::LOADING);
		char* fileData = nullptr;

		std::atomic<fileio::FileStatus> tmpFlag = fileio::FileStatus::LOADING;
		fileio::FileInputOutput::loadBinary(filename, (void**)&fileData, &tmpFlag);

		//Material* materialTmp = new Material();
		
		// .mtl parser is under construction //

		free(fileData);
		//(*material) = (void*)materialTmp;
		flag->store(fileio::FileStatus::READY);
		return;
	}
}

#endif