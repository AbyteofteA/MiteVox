#include "MaterialCodec.h"

namespace fileio
{
	void MaterialCodec::fromGLTF(
		mitevox::Material* materialResult, 
		JSON* materialJSON,
		safety::SafeArray<mitevox::Texture>* textures)
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
					mitevox::Texture* texture = textures->getElementPointer(baseColorTextureIndex);
					materialResult->albedoMap = texture;
				}
				// TODO: texCoord
			}
			if (JSON* numberJSON = pbrMetallicRoughnessJSON->getField("metallicFactor"))
			{
				materialResult->metallicity = (float)numberJSON->getNumberOrDefault(1.0);
			}
			if (JSON* numberJSON = pbrMetallicRoughnessJSON->getField("roughnessFactor"))
			{
				materialResult->roughness = (float)numberJSON->getNumberOrDefault(1.0);
			}
			if (JSON* metallicRoughnessTextureJSON =
				pbrMetallicRoughnessJSON->getField("metallicRoughnessTexture"))
			{
				if (JSON* numberJSON = metallicRoughnessTextureJSON->getField("index"))
				{
					int32_t metallicRoughnessTextureIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
					mitevox::Texture* texture = textures->getElementPointer(metallicRoughnessTextureIndex);
					materialResult->metallicRoughnessMap = texture;
				}
				// TODO: texCoord
			}
		}

		if (JSON* normalMapJSON = materialJSON->getField("normalTexture"))
		{
			if (JSON* numberJSON = normalMapJSON->getField("index"))
			{
				int32_t normalTextureIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
				mitevox::Texture* texture = textures->getElementPointer(normalTextureIndex);
				materialResult->normalMap = texture;
			}
			// TODO: texCoord
		}

		if (JSON* occlusionTextureJSON = materialJSON->getField("occlusionTexture"))
		{
			if (JSON* numberJSON = occlusionTextureJSON->getField("index"))
			{
				int32_t occlusionTextureIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
				mitevox::Texture* texture = textures->getElementPointer(occlusionTextureIndex);
				materialResult->occlusionMap = texture;
			}
			// TODO: texCoord
		}

		if (JSON* emmisiveFactorArrayJSON = materialJSON->getField("emissiveFactor"))
		{
			materialResult->emission.r = (float)emmisiveFactorArrayJSON->getArrayItemNumberOrDefault(0, 0.0);
			materialResult->emission.g = (float)emmisiveFactorArrayJSON->getArrayItemNumberOrDefault(1, 0.0);
			materialResult->emission.b = (float)emmisiveFactorArrayJSON->getArrayItemNumberOrDefault(2, 0.0);
		}

		if (JSON* emmisiveTextureJSON = materialJSON->getField("emissiveTexture"))
		{
			if (JSON* numberJSON = emmisiveTextureJSON->getField("index"))
			{
				int32_t emmisiveTextureIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
				mitevox::Texture* texture = textures->getElementPointer(emmisiveTextureIndex);
				materialResult->emissiveMap = texture;
			}
			// TODO: texCoord
		}
	}

	void MaterialCodec::toGLTF(
		JSON* materialJSONResult,
		mitevox::Material* material,
		safety::SafeArray<mitevox::Texture>* textures)
	{
		size_t texturesCount = textures->getElementsCount();

		materialJSONResult->setField("name", material->name);
		materialJSONResult->setField("doubleSided", material->isDoubleSided);
		materialJSONResult->setField("alphaMode", material->alphaMode);
		// TODO: materialJSONResult->setField("alphaCutoff", (double)material->alphaCutoff);

		JSON* pbrMetallicRoughnessJSON = materialJSONResult->setFieldType("pbrMetallicRoughness", JSONtype::OBJECT);
		JSON* baseColorFactorArrayJSON = pbrMetallicRoughnessJSON->setFieldType("baseColorFactor", JSONtype::ARRAY);
		baseColorFactorArrayJSON->addArrayItem((double)material->baseColor.r);
		baseColorFactorArrayJSON->addArrayItem((double)material->baseColor.g);
		baseColorFactorArrayJSON->addArrayItem((double)material->baseColor.b);
		baseColorFactorArrayJSON->addArrayItem((double)material->baseColor.a);
		if (material->albedoMap)
		{
			JSON* baseColorTextureJSON = pbrMetallicRoughnessJSON->setFieldType("baseColorTexture", JSONtype::OBJECT);
			for (size_t i = 0; i < texturesCount; ++i)
			{
				if (material->albedoMap == textures->getElementPointer(i))
				{
					baseColorTextureJSON->setField("index", (double)i);
					// TODO: texCoord
					break;
				}
			}
		}
		pbrMetallicRoughnessJSON->setField("metallicFactor", (double)material->metallicity);
		pbrMetallicRoughnessJSON->setField("roughnessFactor", (double)material->roughness);
		if (material->metallicRoughnessMap)
		{
			JSON* metallicRoughnessTextureJSON =
				pbrMetallicRoughnessJSON->setFieldType("metallicRoughnessTexture", JSONtype::OBJECT);
			for (size_t i = 0; i < texturesCount; ++i)
			{
				if (material->metallicRoughnessMap == textures->getElementPointer(i))
				{
					metallicRoughnessTextureJSON->setField("index", (double)i);
					// TODO: texCoord
					break;
				}
			}
		}

		if (material->normalMap)
		{
			JSON* normalMapJSON = materialJSONResult->setFieldType("normalTexture", JSONtype::OBJECT);
			for (size_t i = 0; i < texturesCount; ++i)
			{
				if (material->normalMap == textures->getElementPointer(i))
				{
					normalMapJSON->setField("index", (double)i);
					// TODO: texCoord
					break;
				}
			}
		}

		if (material->occlusionMap)
		{
			JSON* occlusionMapJSON = materialJSONResult->setFieldType("occlusionTexture", JSONtype::OBJECT);
			for (size_t i = 0; i < texturesCount; ++i)
			{
				if (material->occlusionMap == textures->getElementPointer(i))
				{
					occlusionMapJSON->setField("index", (double)i);
					// TODO: texCoord
					break;
				}
			}
		}

		JSON* emmisiveFactorArrayJSON = materialJSONResult->setFieldType("emissiveFactor", JSONtype::ARRAY);
		emmisiveFactorArrayJSON->addArrayItem((double)material->emission.r);
		emmisiveFactorArrayJSON->addArrayItem((double)material->emission.g);
		emmisiveFactorArrayJSON->addArrayItem((double)material->emission.b);
		if (material->emissiveMap)
		{
			JSON* emissiveMapJSON = materialJSONResult->setFieldType("emissiveTexture", JSONtype::OBJECT);
			for (size_t i = 0; i < texturesCount; ++i)
			{
				if (material->emissiveMap == textures->getElementPointer(i))
				{
					emissiveMapJSON->setField("index", (double)i);
					// TODO: texCoord
					break;
				}
			}
		}
	}
}