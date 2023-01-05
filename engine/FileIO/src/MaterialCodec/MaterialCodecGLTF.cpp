#include "MaterialCodec.h"

namespace fileio
{
	void MaterialCodec::fromGLTF(
		mitevox::Material* materialResult, 
		JSON* materialJSON,
		safety::SafeArray<mitevox::Texture*>* textures)
	{
		materialResult->name = materialJSON->getFieldStringOrDefault("name", "Untitled");

		if (JSON* numberJSON = materialJSON->getField("doubleSided"))
		{
			materialResult->isDoubleSided = numberJSON->getBooleanOrDefault(false);
		}

		if (JSON* numberJSON = materialJSON->getField("alphaMode"))
		{
			materialResult->alphaMode = numberJSON->getStringOrDefault("OPAQUE");
		}

		if (JSON* numberJSON = materialJSON->getField("alphaCutoff"))
		{
			materialResult->alphaCutoff = numberJSON->getNumberOrDefault(0.5);
		}

		if (JSON* pbrMetallicRoughnessJSON = materialJSON->getField("pbrMetallicRoughness"))
		{
			if (JSON* baseColorFactorArrayJSON = pbrMetallicRoughnessJSON->getField("baseColorFactor"))
			{
				materialResult->baseColor.r = (float)baseColorFactorArrayJSON->getArrayItemNumberOrDefault(0, 1.0);
				materialResult->baseColor.g = (float)baseColorFactorArrayJSON->getArrayItemNumberOrDefault(1, 1.0);
				materialResult->baseColor.b = (float)baseColorFactorArrayJSON->getArrayItemNumberOrDefault(2, 1.0);
				materialResult->baseColor.a = (float)baseColorFactorArrayJSON->getArrayItemNumberOrDefault(3, 1.0);
			}
			if (JSON* baseColorTextureJSON = pbrMetallicRoughnessJSON->getField("baseColorTexture"))
			{
				JSON* numberJSON = baseColorTextureJSON->getField("index");
				if (numberJSON != nullptr)
				{
					int32_t baseColorTextureIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
					mitevox::Texture* texture = textures->getElement(baseColorTextureIndex);
					materialResult->albedoMap = texture;
				}
			}
			if (JSON* numberJSON = pbrMetallicRoughnessJSON->getField("metallicFactor"))
			{
				materialResult->metallicity = (float)numberJSON->getNumberOrDefault(1.0);
			}
			if (JSON* numberJSON = pbrMetallicRoughnessJSON->getField("roughnessFactor"))
			{
				materialResult->roughness = (float)numberJSON->getNumberOrDefault(1.0);
			}
			if (JSON* pbrMetallicRoughnessTextureJSON =
				pbrMetallicRoughnessJSON->getField("pbrMetallicRoughnessTexture"))
			{
				if (JSON* numberJSON = pbrMetallicRoughnessTextureJSON->getField("index"))
				{
					int32_t pbrMetallicRoughnessTextureIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
					mitevox::Texture* texture = textures->getElement(pbrMetallicRoughnessTextureIndex);
					materialResult->metallicRoughnessMap = texture;
				}
			}
		}

		if (JSON* normalMapJSON = materialJSON->getField("normalTexture"))
		{
			if (JSON* numberJSON = normalMapJSON->getField("index"))
			{
				int32_t normalTextureIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
				mitevox::Texture* texture = textures->getElement(normalTextureIndex);
				materialResult->normalMap = texture;
			}
		}

		if (JSON* occlusionTextureJSON = materialJSON->getField("occlusionTexture"))
		{
			if (JSON* numberJSON = occlusionTextureJSON->getField("index"))
			{
				int32_t occlusionTextureIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
				mitevox::Texture* texture = textures->getElement(occlusionTextureIndex);
				materialResult->occlusionMap = texture;
			}
		}

		if (JSON* emmisiveFactorArrayJSON = materialJSON->getField("emissiveFactor"))
		{
			materialResult->emission.r = (float)emmisiveFactorArrayJSON->getArrayItemNumberOrDefault(0, 0.0);
			materialResult->emission.g = (float)emmisiveFactorArrayJSON->getArrayItemNumberOrDefault(1, 0.0);
			materialResult->emission.b = (float)emmisiveFactorArrayJSON->getArrayItemNumberOrDefault(2, 0.0);
		}

		if (JSON* emmisiveTextureJSON = materialJSON->getField("baseColorTexture"))
		{
			if (JSON* numberJSON = emmisiveTextureJSON->getField("index"))
			{
				int32_t emmisiveTextureIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
				mitevox::Texture* texture = textures->getElement(emmisiveTextureIndex);
				materialResult->emissiveMap = texture;
			}
		}
	}
}