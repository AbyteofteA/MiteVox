#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

#include "engine/MiteVox/src/Material/Material.h"
#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/MiteVox/src/Skeleton/SkeletonBase.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/Math/src/Utilities/Convertations.h"

#include <vector>
#include <string>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace render
{
	size_t screenQuadVBO = 0;
	size_t screenQuadID = 0;
	size_t getScreenQuadID()
	{
		if (screenQuadID == 0)
		{
			glGenBuffers(1, &screenQuadVBO); // TODO: glDeleteBuffers(1, &screenQuadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
			glGenVertexArrays(1, &screenQuadID);
			glBindVertexArray(screenQuadID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, screenQuad, GL_STATIC_DRAW);
			glEnableVertexAttribArray(DEFAULT_ATTRIBUTE_POSITION);
			glVertexAttribPointer(DEFAULT_ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(DEFAULT_ATTRIBUTE_TEX_COORD_0);
			glVertexAttribPointer(DEFAULT_ATTRIBUTE_TEX_COORD_0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		return screenQuadID;
	}

	size_t unitCubeVBO = 0;
	size_t unitCubeID = 0;
	size_t getUnitCubeID()
	{
		if (unitCubeID == 0)
		{
			glGenBuffers(1, &unitCubeVBO); // TODO: glDeleteBuffers(1, &unitCubeVBO);
			glBindBuffer(GL_ARRAY_BUFFER, unitCubeVBO);
			glGenVertexArrays(1, &unitCubeID);
			glBindVertexArray(unitCubeID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, unitCubePositions, GL_STATIC_DRAW);
			glEnableVertexAttribArray(DEFAULT_ATTRIBUTE_POSITION);
			glVertexAttribPointer(DEFAULT_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		return unitCubeID;
	}

	void resetLights(int shaderID)
	{
		if (!shaders[shaderID]->use())
			return;

		shaders[shaderID]->setVector3D("ambientLight", mathem::Vector3D::zero());
		shaders[shaderID]->setInt("amountOfDirectionalLights", 0);
		shaders[shaderID]->setInt("amountOfPointLights", 0);
		shaders[shaderID]->setInt("amountOfSpotLights", 0);
	}

	void setAmbientLight(mathem::Vector3D ambientLightColor, int shaderID)
	{
		if (!shaders[shaderID]->use())
			return;

		shaders[shaderID]->setVector3D("ambientLight", ambientLightColor);
		PRINT_RENDERER_ERRORS;
	}

	void uploadDirectionalLights(safety::SafeArray<DirectionalLight>* lightsArray, size_t offset, size_t count, int shaderID)
	{
		if (!shaders[shaderID]->use())
			return;

		size_t remainingDirectionalLightsCount = lightsArray->getElementsCount() - offset;
		count = std::min(count, remainingDirectionalLightsCount);
		shaders[shaderID]->setInt("amountOfDirectionalLights", count);

		for (size_t i = 0; i < count; ++i)
		{
			DirectionalLight directionalLight = lightsArray->getElement(i + offset);

			std::string indexStr = std::to_string(i);
			static const std::string directionalLights = "directionalLights[";
			static const std::string direction = "].direction";
			static const std::string color = "].color";
			static const std::string intensity = "].intensity";
			std::string directionResult = directionalLights + indexStr + direction;
			std::string colorResult = directionalLights + indexStr + color;
			std::string intensityResult = directionalLights + indexStr + intensity;

			shaders[shaderID]->setVec3(
				directionResult.c_str(),
				directionalLight.direction.x(),
				directionalLight.direction.y(),
				directionalLight.direction.z());
			shaders[shaderID]->setVec3(
				colorResult.c_str(),
				directionalLight.lightBase.color.r,
				directionalLight.lightBase.color.g,
				directionalLight.lightBase.color.b);

			shaders[shaderID]->setFloat(intensityResult.c_str(), directionalLight.lightBase.intensity);
		}
	}

	void uploadPointLights(safety::SafeArray<PointLight>* lightsArray, size_t offset, size_t count, int shaderID)
	{
		if (!shaders[shaderID]->use())
			return;

		size_t remainingPointLightsCount = lightsArray->getElementsCount() - offset;
		count = std::min(count, remainingPointLightsCount);

		shaders[shaderID]->setInt("amountOfPointLights", count);

		for (size_t i = 0; i < count; ++i)
		{
			PointLight pointLight = lightsArray->getElement(i + offset);
			
			std::string indexStr = std::to_string(i);
			static const std::string pointLights = "pointLights[";
			static const std::string pos = "].pos";
			static const std::string color = "].color";
			static const std::string intensity = "].intensity";
			static const std::string range = "].range";
			std::string posResult = pointLights + indexStr + pos;
			std::string colorResult = pointLights + indexStr + color;
			std::string intensityResult = pointLights + indexStr + intensity;
			std::string rangeResult = pointLights + indexStr + range;

			shaders[shaderID]->setVec3(
				posResult.c_str(), 
				pointLight.position.x(), 
				pointLight.position.y(), 
				pointLight.position.z());
			shaders[shaderID]->setVec3(
				colorResult.c_str(), 
				pointLight.lightBase.color.r, 
				pointLight.lightBase.color.g, 
				pointLight.lightBase.color.b);
			
			shaders[shaderID]->setFloat(intensityResult.c_str(), pointLight.lightBase.intensity);
			shaders[shaderID]->setFloat(rangeResult.c_str(), pointLight.lightBase.range);
		}
	}

	void uploadSpotLights(safety::SafeArray<render::SpotLight>* lightsArray, size_t offset, size_t count, int shaderID)
	{
		if (!shaders[shaderID]->use())
			return;

		size_t remainingSpotLightsCount = lightsArray->getElementsCount() - offset;
		count = std::min(count, remainingSpotLightsCount);

		shaders[shaderID]->setInt("amountOfSpotLights", count);

		for (size_t i = 0; i < count; ++i)
		{
			render::SpotLight spotLight = lightsArray->getElement(i + offset);

			std::string indexStr = std::to_string(i);
			static const std::string spotLights = "spotLights[";
			static const std::string pos = "].pos";
			static const std::string direction = "].direction";
			static const std::string innerConeAngle = "].innerConeAngle";
			static const std::string outerConeAngle = "].outerConeAngle";
			static const std::string color = "].color";
			static const std::string intensity = "].intensity";
			static const std::string range = "].range";
			std::string posResult = spotLights + indexStr + pos;
			std::string directionResult = spotLights + indexStr + direction;
			std::string innerConeAngleResult = spotLights + indexStr + innerConeAngle;
			std::string outerConeAngleResult = spotLights + indexStr + outerConeAngle;
			std::string colorResult = spotLights + indexStr + color;
			std::string intensityResult = spotLights + indexStr + intensity;
			std::string rangeResult = spotLights + indexStr + range;

			shaders[shaderID]->setVec3(
				posResult.c_str(),
				spotLight.position.x(),
				spotLight.position.y(),
				spotLight.position.z());
			shaders[shaderID]->setVec3(
				directionResult.c_str(),
				spotLight.direction.x(),
				spotLight.direction.y(),
				spotLight.direction.z());

			shaders[shaderID]->setFloat(innerConeAngleResult.c_str(), spotLight.innerConeAngle);
			shaders[shaderID]->setFloat(outerConeAngleResult.c_str(), spotLight.outerConeAngle);

			shaders[shaderID]->setVec3(
				colorResult.c_str(),
				spotLight.lightBase.color.r,
				spotLight.lightBase.color.g,
				spotLight.lightBase.color.b);

			shaders[shaderID]->setFloat(intensityResult.c_str(), spotLight.lightBase.intensity);
			shaders[shaderID]->setFloat(rangeResult.c_str(), spotLight.lightBase.range);
		}
		PRINT_RENDERER_ERRORS;
	}

	void uploadMaterial(mitevox::Material* material)
	{
		if (mitevox::Texture* albedoMap = material->albedoMap)
		{
			if (albedoMap->ID == 0)
			{
				glGenTextures(1, &albedoMap->ID);
				glBindTexture(GL_TEXTURE_2D, albedoMap->ID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, albedoMap->sampler->wrappingModeU);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, albedoMap->sampler->wrappingModeV);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, albedoMap->sampler->magnificationFilter);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					albedoMap->getWidth(),
					albedoMap->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 
					albedoMap->image->imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		if (mitevox::Texture* metallicRoughnessMap = material->metallicRoughnessMap)
		{
			if (metallicRoughnessMap->ID == 0)
			{
				glGenTextures(1, &metallicRoughnessMap->ID);
				glBindTexture(GL_TEXTURE_2D, metallicRoughnessMap->ID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, metallicRoughnessMap->sampler->wrappingModeU);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, metallicRoughnessMap->sampler->wrappingModeV);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, metallicRoughnessMap->sampler->magnificationFilter);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					metallicRoughnessMap->getWidth(),
					metallicRoughnessMap->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 
					metallicRoughnessMap->image->imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		if (mitevox::Texture* normalMap = material->normalMap)
		{
			if (normalMap->ID == 0)
			{
				glGenTextures(1, &normalMap->ID);
				glBindTexture(GL_TEXTURE_2D, normalMap->ID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, normalMap->sampler->wrappingModeU);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, normalMap->sampler->wrappingModeV);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, normalMap->sampler->magnificationFilter);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					normalMap->getWidth(),
					normalMap->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
					normalMap->image->imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		if (mitevox::Texture* occlusionMap = material->occlusionMap)
		{
			if (occlusionMap->ID == 0)
			{
				glGenTextures(1, &occlusionMap->ID);
				glBindTexture(GL_TEXTURE_2D, occlusionMap->ID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, occlusionMap->sampler->wrappingModeU);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, occlusionMap->sampler->wrappingModeV);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, occlusionMap->sampler->magnificationFilter);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					occlusionMap->getWidth(),
					occlusionMap->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
					occlusionMap->image->imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		if (mitevox::Texture* emissiveMap = material->emissiveMap)
		{
			if (emissiveMap->ID == 0)
			{
				glGenTextures(1, &emissiveMap->ID);
				glBindTexture(GL_TEXTURE_2D, emissiveMap->ID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, emissiveMap->sampler->wrappingModeU);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, emissiveMap->sampler->wrappingModeV);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, emissiveMap->sampler->magnificationFilter);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					emissiveMap->getWidth(),
					emissiveMap->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
					emissiveMap->image->imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		PRINT_RENDERER_ERRORS;
	}

	void selectMaterial(mitevox::Material* material, int shaderID)
	{
		glm::vec3 baseColor = { material->baseColor.r, material->baseColor.g, material->baseColor.b };
		shaders[shaderID]->setVec3("material.baseColor", baseColor);
		shaders[shaderID]->setFloat("material.roughness", material->roughness);
		shaders[shaderID]->setFloat("material.metallicity", material->metallicity);
		shaders[shaderID]->setFloat("material.specularExponent", material->specularExponent);
		glm::vec3 emissionColor = { material->emission.r, material->emission.g, material->emission.b };
		shaders[shaderID]->setVec3("material.emission", emissionColor);

		if (material->albedoMap != nullptr)
		{
			shaders[shaderID]->setBool("material.hasAlbedoMap", true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_ALBEDO);
			glBindTexture(GL_TEXTURE_2D, material->albedoMap->ID);
			shaders[shaderID]->setInt("albedoMap", TEXTURE_UNIT_ALBEDO);
		}
		else
		{
			shaders[shaderID]->setBool("material.hasAlbedoMap", false);
		}

		if (material->metallicRoughnessMap != nullptr)
		{
			shaders[shaderID]->setBool("material.hasMetallicRoughnessMap", true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_METALLIC_ROUGHNESS);
			glBindTexture(GL_TEXTURE_2D, material->metallicRoughnessMap->ID);
			shaders[shaderID]->setInt("metallicRoughnessMap", TEXTURE_UNIT_METALLIC_ROUGHNESS);
		}
		else
		{
			shaders[shaderID]->setBool("material.hasMetallicRoughnessMap", false);
		}

		if (material->normalMap != nullptr)
		{
			shaders[shaderID]->setBool("material.hasNormalMap", true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_NORMAL);
			glBindTexture(GL_TEXTURE_2D, material->normalMap->ID);
			shaders[shaderID]->setInt("normalMap", TEXTURE_UNIT_NORMAL);
		}
		else
		{
			shaders[shaderID]->setBool("material.hasNormalMap", false);
		}
		if (material->occlusionMap != nullptr)
		{
			shaders[shaderID]->setBool("material.hasOcclusionMap", true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_OCCLUSION);
			glBindTexture(GL_TEXTURE_2D, material->occlusionMap->ID);
			shaders[shaderID]->setInt("occlusionMap", TEXTURE_UNIT_OCCLUSION);
		}
		else
		{
			shaders[shaderID]->setBool("material.hasOcclusionMap", false);
		}
		if (material->emissiveMap != nullptr)
		{
			shaders[shaderID]->setBool("material.hasEmissiveMap", true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_EMISSIVE);
			glBindTexture(GL_TEXTURE_2D, material->emissiveMap->ID);
			shaders[shaderID]->setInt("emissiveMap", TEXTURE_UNIT_EMISSIVE);
		}
		else
		{
			shaders[shaderID]->setBool("material.hasEmissiveMap", false);
		}

		shaders[shaderID]->setInt("material.illuminationModel", (int)material->illuminationModel);

		PRINT_RENDERER_ERRORS;
	}

	void removeMaterial(mitevox::Material* material)
	{
		glDeleteTextures(1, &material->albedoMap->ID);
		glDeleteTextures(1, &material->metallicRoughnessMap->ID);
		glDeleteTextures(1, &material->normalMap->ID);
		glDeleteTextures(1, &material->occlusionMap->ID);
		glDeleteTextures(1, &material->emissiveMap->ID);
	}

	void uploadBufferView(mitevox::BufferView* bufferView)
	{
		if (bufferView->ID == 0)
		{
			glGenBuffers(1, &bufferView->ID);
			
			uint8_t* bufferViewData = (uint8_t*)bufferView->getDataStart();

			if (bufferView->target == mitevox::BufferView::ARRAY_BUFFER)
			{
				glBindBuffer(GL_ARRAY_BUFFER, bufferView->ID);
				glBufferData(GL_ARRAY_BUFFER, bufferView->byteLength, 0, GL_DYNAMIC_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, bufferView->byteLength, bufferViewData);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else if (bufferView->target == mitevox::BufferView::ELEMENT_ARRAY_BUFFER)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferView->ID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferView->byteLength, 0, GL_DYNAMIC_DRAW);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, bufferView->byteLength, bufferViewData);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}
		PRINT_RENDERER_ERRORS;
	}

	void updateBufferView(mitevox::BufferView* bufferView)
	{
		if (bufferView->ID != 0)
		{
			uint8_t* bufferViewData = (uint8_t*)bufferView->getDataStart();

			if (bufferView->target == mitevox::BufferView::ARRAY_BUFFER)
			{
				glBindBuffer(GL_ARRAY_BUFFER, bufferView->ID);
				GLint size = 0;
				glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
				glBufferSubData(GL_ARRAY_BUFFER, 0, bufferView->byteLength, bufferViewData);
			}
			else if (bufferView->target == mitevox::BufferView::ELEMENT_ARRAY_BUFFER)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferView->ID);
				GLint size = 0;
				glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, bufferView->byteLength, bufferViewData);
			}
		}
		PRINT_RENDERER_ERRORS;
	}

	void removeBufferView(mitevox::BufferView* bufferView)
	{
		glDeleteBuffers(1, &bufferView->ID);
	}

	void checkBufferView(mitevox::BufferView* bufferView)
	{
		if (bufferView->ID != 0)
		{
			if (bufferView->target == mitevox::BufferView::ARRAY_BUFFER)
			{
				glBindBuffer(GL_ARRAY_BUFFER, bufferView->ID);

				GLint size = 0;
				glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
				if (bufferView->byteLength != size)
				{
					std::cout << "GL_ARRAY_BUFFER " << bufferView->ID << " byteLength == " << size << std::endl;
				}
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else if (bufferView->target == mitevox::BufferView::ELEMENT_ARRAY_BUFFER)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferView->ID);

				GLint size = 0;
				glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
				if (bufferView->byteLength != size)
				{
					std::cout << "GL_ELEMENT_ARRAY_BUFFER " << bufferView->ID << " byteLength == " << size << std::endl;
				}
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}
	}

	void checkMeshBuffers(mitevox::Mesh* mesh)
	{
		int64_t meshesPrimitivesCount = mesh->primitives.getElementsCount();
		for (int64_t primitiveIndex = 0; primitiveIndex < meshesPrimitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* meshPrimitive = mesh->primitives.getElement(primitiveIndex);

			for (size_t i = 0; i < mitevox::MeshAttributeSet::attributesCount; ++i)
			{
				if (auto attributeAccessor = meshPrimitive->attributes.byIndex[i])
				{
					checkBufferView(attributeAccessor->bufferView);
				}
			}

			if (auto indexAccessor = meshPrimitive->getIndeces())
			{
				checkBufferView(indexAccessor->bufferView);
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	mitevox::SkeletonBase* tryUploadSkeleton(mitevox::Node* node, int shaderID)
	{
		if (!shaders[shaderID]->use())
			return nullptr;

		if (mitevox::SkeletonBase* skeleton = (mitevox::SkeletonBase*)node->skeleton)
		{
			shaders[shaderID]->setInt("jointsCount", skeleton->jointMatrices.getElementsCount());
			shaders[shaderID]->setMatrix4x4Array("jointMatrices", &skeleton->jointMatrices);
			return skeleton;
		}
		
		shaders[shaderID]->setInt("jointsCount", 0);
		return nullptr;
	}

	void uploadMesh(mitevox::Mesh* mesh)
	{
		int64_t meshesPrimitivesCount = mesh->primitives.getElementsCount();
		for (int64_t primitiveIndex = 0; primitiveIndex < meshesPrimitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* meshPrimitive = mesh->primitives.getElement(primitiveIndex);

			for (size_t i = 0; i < mitevox::MeshAttributeSet::attributesCount; ++i)
			{
				if (auto attributeAccessor = meshPrimitive->attributes.byIndex[i])
				{
					uploadBufferView(attributeAccessor->bufferView);
				}
			}

			if (auto indexAccessor = meshPrimitive->getIndeces())
			{
				uploadBufferView(indexAccessor->bufferView);
			}

			glGenVertexArrays(1, &meshPrimitive->ID);
			glBindVertexArray(meshPrimitive->ID);

#define ATTRIBUTE_LOCATION_POSITION 0
#define ATTRIBUTE_LOCATION_NORNAL 1
#define ATTRIBUTE_LOCATION_TEXTURE_COORD_0 2
#define ATTRIBUTE_LOCATION_TEXTURE_COORD_1 3
#define ATTRIBUTE_LOCATION_COLOR 4
#define ATTRIBUTE_LOCATION_JOINTS 5
#define ATTRIBUTE_LOCATION_WEIGHTS 6
#define ATTRIBUTE_LOCATION_TANGENTS 7

			for (size_t i = 0; i < mitevox::MeshAttributeSet::attributesCount; ++i)
			{
				if (auto attributeAccessor = meshPrimitive->attributes.byIndex[i])
				{
					if (i != ATTRIBUTE_LOCATION_JOINTS)
					{
						const size_t attributeLocation = i;
						glEnableVertexAttribArray(attributeLocation);
						glBindBuffer(GL_ARRAY_BUFFER, attributeAccessor->bufferView->ID);
						glVertexAttribPointer(
							attributeLocation,
							attributeAccessor->getComponentsPerElement(),
							attributeAccessor->componentType,
							attributeAccessor->normalized,
							attributeAccessor->bufferView->byteStride,
							(void*)attributeAccessor->byteOffset);
					}
					else
					{
						const size_t attributeLocation = ATTRIBUTE_LOCATION_JOINTS;
						glEnableVertexAttribArray(attributeLocation);
						glBindBuffer(GL_ARRAY_BUFFER, attributeAccessor->bufferView->ID);
						glVertexAttribIPointer(
							attributeLocation,
							attributeAccessor->getComponentsPerElement(),
							attributeAccessor->componentType,
							attributeAccessor->bufferView->byteStride,
							(void*)attributeAccessor->byteOffset);
					}
					
				}
			}

			if (auto indexAccessor = meshPrimitive->getIndeces())
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexAccessor->bufferView->ID);
			}
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			uploadMaterial(meshPrimitive->material);

			PRINT_RENDERER_ERRORS;
		}

		mesh->isUploaded = true;

		checkMeshBuffers(mesh);
	}

	void updateMesh(mitevox::Mesh* mesh)
	{
		int64_t meshesPrimitivesCount = mesh->primitives.getElementsCount();
		for (int64_t primitiveIndex = 0; primitiveIndex < meshesPrimitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* meshPrimitive = mesh->primitives.getElement(primitiveIndex);

			for (size_t i = 0; i < mitevox::MeshAttributeSet::attributesCount; ++i)
			{
				if (auto attributeAccessor = meshPrimitive->attributes.byIndex[i])
				{
					updateBufferView(attributeAccessor->bufferView);
				}
			}

			if (auto indexAccessor = meshPrimitive->getIndeces())
			{
				updateBufferView(indexAccessor->bufferView);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// TODO: updateMaterial(meshPrimitive->material, shaderID);

			PRINT_RENDERER_ERRORS;
		}

		mesh->isUploaded = true;
	}

	void removeMesh(mitevox::Mesh* mesh)
	{
		int64_t meshesPrimitivesCount = mesh->primitives.getElementsCount();
		for (int64_t primitiveIndex = 0; primitiveIndex < meshesPrimitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* meshPrimitive = mesh->primitives.getElement(primitiveIndex);
			glDeleteVertexArrays(1, &meshPrimitive->ID);

			for (size_t i = 0; i < mitevox::MeshAttributeSet::attributesCount; ++i)
			{
				if (auto attributeAccessor = meshPrimitive->attributes.byIndex[i])
				{
					removeBufferView(attributeAccessor->bufferView);
				}
			}

			if (auto indexAccessor = meshPrimitive->getIndeces())
			{
				removeBufferView(indexAccessor->bufferView);
			}
		}
		mesh->isUploaded = false;
	}

	void renderMesh(
		RendererSettings* renderer,
		int shaderID,
		mitevox::Mesh* mesh,
		mathem::GeometryTransform* transform,
		Camera* camera,
		mathem::GeometryTransform* cameraTransform)
	{
		if (!shaders[shaderID]->use())
			return;

		checkMeshBuffers(mesh);

		shaders[shaderID]->setVec3("viewPos", 
			cameraTransform->translation.x(), 
			cameraTransform->translation.y(), 
			cameraTransform->translation.z());

		mathem::Matrix4x4 modelMatrix = mathem::transformToMatrix(*transform);
		shaders[shaderID]->setMatrix4x4("modelMatrix", modelMatrix);
		shaders[shaderID]->setCameraMatrices(camera);

		int64_t meshesPrimitivesCount = mesh->primitives.getElementsCount();
		for (int64_t primitiveIndex = 0; primitiveIndex < meshesPrimitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* meshPrimitive = mesh->primitives.getElement(primitiveIndex);

			renderer->amountOfDrawCalls++;

			glBindVertexArray(meshPrimitive->ID);
			selectMaterial(meshPrimitive->material, shaderID);

			if (auto indexAccessor = meshPrimitive->getIndeces())
			{
				glDrawElements(GL_TRIANGLES, indexAccessor->count, indexAccessor->componentType, (GLvoid*)indexAccessor->byteOffset);
			}
			else
			{
				size_t vertecesCount = meshPrimitive->attributes.byName.positionAccessor->count;
				glDrawArrays(GL_TRIANGLES, 0, vertecesCount);
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			PRINT_RENDERER_ERRORS;
		}
	}

	extern float skyboxVertices[];

	void uploadSkybox(Cubemap* skybox, int shaderID)
	{
		if (skybox->vertexID == 0)
		{
			GLint posAttrib = glGetAttribLocation(shaders[shaderID]->shaderID, "position");

			unsigned int skyboxVBO;
			glGenVertexArrays(1, &skybox->vertexID);
			glGenBuffers(1, &skyboxVBO);
			glBindVertexArray(skybox->vertexID);
			glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(posAttrib);
			glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

			glGenTextures(1, &skybox->textureID);
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->textureID);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			for (size_t i = 0; i < 6; i++)
			{
				char* image = (char*)skybox->textures[i].imageData;
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
					skybox->textures[i].width,
					skybox->textures[i].height,
					0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			}

			shaders[shaderID]->setInt("skybox", 0);
		}

		PRINT_RENDERER_ERRORS;
	}

	void selectSkybox(Cubemap* skybox, int shaderID)
	{
		if (skybox == nullptr)
		{
			return;
		}
		glBindVertexArray(skybox->vertexID);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->textureID);

		PRINT_RENDERER_ERRORS;
	}

	void removeSkybox(Cubemap* skybox, int shaderID)
	{
		if (skybox == nullptr)
		{
			return;
		}
		glDeleteBuffers(1, &skybox->vertexID);
		glDeleteTextures(1, &skybox->textureID);
	}

	void renderSkybox(
		RendererSettings* renderer, 
		int shaderID, 
		Cubemap* skybox, 
		Camera* camera, 
		mathem::GeometryTransform* cameraTransform)
	{
		if (!shaders[shaderID]->use())
			return;

		if (skybox->vertexID == 0)
		{
			uploadSkybox(skybox, skybox->shaderID);
		}

		mathem::GeometryTransform cameraTransformOrientationOnly;
		cameraTransformOrientationOnly.rotation = cameraTransform->rotation;
		glm::mat4 viewMatrix = camera->makeViewMatrix(&cameraTransformOrientationOnly);
		glm::mat4 projectionMatrix = camera->getProjectionMatrix();
		shaders[shaderID]->setMat4("viewProjectionMatrix", projectionMatrix * viewMatrix);

		renderer->amountOfDrawCalls++;

		selectSkybox(skybox, shaderID);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		PRINT_RENDERER_ERRORS;
	}
}
