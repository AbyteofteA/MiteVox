#include "MaterialCodec.h"

namespace fileio
{
	void MaterialCodec::fromGLTF(
		mitevox::Material* materialResult, 
		JSON* materialJSON,
		safety::SafeArray<mitevox::Texture*>* textures)
	{
		materialResult->name = materialJSON->getFieldString("name");

		JSON* pbrMetallicRoughnessJSON = materialJSON->getField("pbrMetallicRoughness");
		if (pbrMetallicRoughnessJSON != nullptr)
		{
			JSON* baseColorFactorArrayJSON = pbrMetallicRoughnessJSON->getField("baseColorFactor");
			if (baseColorFactorArrayJSON != nullptr)
			{
				materialResult->baseColor.r = (float)baseColorFactorArrayJSON->getArrayItem(0)->getNumber();
				materialResult->baseColor.g = (float)baseColorFactorArrayJSON->getArrayItem(1)->getNumber();
				materialResult->baseColor.b = (float)baseColorFactorArrayJSON->getArrayItem(2)->getNumber();
				materialResult->baseColor.a = (float)baseColorFactorArrayJSON->getArrayItem(3)->getNumber();
			}
			JSON* baseColorTextureJSON = pbrMetallicRoughnessJSON->getField("baseColorTexture");
			if (baseColorTextureJSON != nullptr)
			{
				JSON* numberJSON = baseColorTextureJSON->getField("index");
				if (numberJSON != nullptr)
				{
					if (numberJSON->isNumber())
					{
						int32_t baseColorTextureIndex = (int32_t)numberJSON->getNumber();
						mitevox::Texture* texture = textures->getElement(baseColorTextureIndex);
						materialResult->albedoMap = texture;
					}
				}
			}
			JSON* numberJSON = pbrMetallicRoughnessJSON->getField("metallicFactor");
			if (numberJSON != nullptr)
			{
				if (numberJSON->isNumber())
				{
					materialResult->metallicity = (float)numberJSON->getNumber();
				}
			}
			numberJSON = pbrMetallicRoughnessJSON->getField("roughnessFactor");
			if (numberJSON != nullptr)
			{
				if (numberJSON->isNumber())
				{
					materialResult->roughness = (float)numberJSON->getNumber();
				}
			}
			JSON* pbrMetallicRoughnessTextureJSON = 
				pbrMetallicRoughnessJSON->getField("pbrMetallicRoughnessTexture");
			if (pbrMetallicRoughnessTextureJSON != nullptr)
			{
				JSON* numberJSON = pbrMetallicRoughnessTextureJSON->getField("index");
				if (numberJSON != nullptr)
				{
					if (numberJSON->isNumber())
					{
						int32_t pbrMetallicRoughnessTextureIndex = (int32_t)numberJSON->getNumber();
						mitevox::Texture* texture = textures->getElement(pbrMetallicRoughnessTextureIndex);
						materialResult->metallicRoughnessMap = texture;
					}
				}
			}
		}
	}
}