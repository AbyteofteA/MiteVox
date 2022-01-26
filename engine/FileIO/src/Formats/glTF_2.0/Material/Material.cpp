#include "Material.h"

namespace fileio
{
	void Material::fromGLTF(JSON* materialJSON, safety::SafeArray<Texture*>* textures)
	{
		name = materialJSON->getFieldString("name");

		JSON* pbrMetallicRoughnessJSON = materialJSON->getField("pbrMetallicRoughness");
		if (pbrMetallicRoughnessJSON != nullptr)
		{
			JSON* baseColorFactorArrayJSON = pbrMetallicRoughnessJSON->getField("baseColorFactor");
			if (baseColorFactorArrayJSON != nullptr)
			{
				baseColor.r = (float)baseColorFactorArrayJSON->getArrayItem(0)->getNumber();
				baseColor.g = (float)baseColorFactorArrayJSON->getArrayItem(1)->getNumber();
				baseColor.b = (float)baseColorFactorArrayJSON->getArrayItem(2)->getNumber();
				baseColor.a = (float)baseColorFactorArrayJSON->getArrayItem(3)->getNumber();
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
						Texture* texture = textures->getElement(baseColorTextureIndex);
						albedoMap = texture;
					}
				}
			}
			JSON* numberJSON = pbrMetallicRoughnessJSON->getField("metallicFactor");
			if (numberJSON != nullptr)
			{
				if (numberJSON->isNumber())
				{
					metallicity = (float)numberJSON->getNumber();
				}
			}
			numberJSON = pbrMetallicRoughnessJSON->getField("roughnessFactor");
			if (numberJSON != nullptr)
			{
				if (numberJSON->isNumber())
				{
					roughness = (float)numberJSON->getNumber();
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
						Texture* texture = textures->getElement(pbrMetallicRoughnessTextureIndex);
						metallicRoughnessMap = texture;
					}
				}
			}
		}
	}
}