
#include <vector>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "API_ModelFunctions.h"

#include "engine/MiteVox/src/Material/Material.h"
#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/MiteVox/src/Skeleton/SkeletonBase.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"

namespace render
{
	extern float skyboxVert[];

	void uploadSkybox(Skybox* skybox)
	{
		GLint posAttrib = glGetAttribLocation(shaders[1]->shaderID, "position");

		unsigned int skyboxVBO;
		glGenVertexArrays(1, &skybox->vertexID);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skybox->vertexID);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, skyboxVert, GL_STATIC_DRAW);
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		glGenTextures(1, &skybox->cubemap->textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemap->textureID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

		for (int i = 0; i < 6; i++)
		{
			char* image = (char*)skybox->cubemap->textures[i].imageData;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, 
				skybox->cubemap->textures[i].width, 
				skybox->cubemap->textures[i].height, 
				0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}
	}

	void uploadPointLights(safety::SafeArray<render::PointLight>* lightsArray, int shaderID)
	{
		if (!render::shaders[shaderID]->use())
			return;

		size_t lightsCount = lightsArray->getElementsCount();
		render::shaders[shaderID]->setInt("amountOfPointLights", lightsCount);

		for (size_t i = 0; i < lightsCount; ++i)
		{
			render::PointLight pointLight = lightsArray->getElement(i);
			
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

			render::shaders[shaderID]->setVec3(
				posResult.c_str(), 
				pointLight.position.x(), 
				pointLight.position.y(), 
				pointLight.position.z());
			render::shaders[shaderID]->setVec3(
				colorResult.c_str(), 
				pointLight.lightBase.color.r, 
				pointLight.lightBase.color.g, 
				pointLight.lightBase.color.b);
			
			render::shaders[shaderID]->setFloat(intensityResult.c_str(), pointLight.lightBase.intensity);
			render::shaders[shaderID]->setFloat(rangeResult.c_str(), pointLight.lightBase.range);
		}
	}

	void clearPointLights(int shaderID)
	{
		if (!render::shaders[shaderID]->use())
			return;

		render::shaders[shaderID]->setInt("amountOfPointLights", 0);
	}

	void uploadMaterial(mitevox::Material* material, int shaderID)
	{
		int textureUnit = 0;
		mitevox::Texture* albedoMap = material->albedoMap;
		if (albedoMap != nullptr)
		{
			if (albedoMap->ID == 0)
			{
				glGenTextures(1, &albedoMap->ID);
				glActiveTexture(GL_TEXTURE0 + textureUnit);
				glBindTexture(GL_TEXTURE_2D, albedoMap->ID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, albedoMap->sampler.wrappingModeU);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, albedoMap->sampler.wrappingModeV);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, albedoMap->sampler.minificationFilter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, albedoMap->sampler.magnificationFilter);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					albedoMap->getWidth(),
					albedoMap->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 
					albedoMap->image->imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
				glUniform1i(glGetUniformLocation(shaders[shaderID]->shaderID, "albedoMap"), textureUnit);
			}
			textureUnit++;
		}
		mitevox::Texture* metallicRoughnessMap = material->metallicRoughnessMap;
		if (metallicRoughnessMap != nullptr)
		{
			if (metallicRoughnessMap->ID == 0)
			{
				glGenTextures(1, &metallicRoughnessMap->ID);
				glActiveTexture(GL_TEXTURE0 + textureUnit);
				glBindTexture(GL_TEXTURE_2D, metallicRoughnessMap->ID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, metallicRoughnessMap->sampler.wrappingModeU);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, metallicRoughnessMap->sampler.wrappingModeV);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, metallicRoughnessMap->sampler.minificationFilter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, metallicRoughnessMap->sampler.magnificationFilter);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					metallicRoughnessMap->getWidth(),
					metallicRoughnessMap->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 
					metallicRoughnessMap->image->imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
				glUniform1i(glGetUniformLocation(shaders[shaderID]->shaderID, "metallicRoughnessMap"), textureUnit);
			}
			textureUnit++;
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

		int textureUnit = 0;
		if (material->albedoMap != nullptr)
		{
			shaders[shaderID]->setBool("material.hasAlbedoMap", true);
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, material->albedoMap->ID);
			textureUnit++;
		}
		else
		{
			shaders[shaderID]->setBool("material.hasAlbedoMap", false);
		}

		if (material->metallicRoughnessMap != nullptr)
		{
			shaders[shaderID]->setBool("material.hasMetallicRoughnessMap", true);
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, material->metallicRoughnessMap->ID);
			textureUnit++;
		}
		else
		{
			shaders[shaderID]->setBool("material.hasMetallicRoughnessMap", false);
		}

		shaders[shaderID]->setInt("material.illuminationModel", material->illuminationModel);

		PRINT_RENDERER_ERRORS;
	}

	void removeMaterial(mitevox::Material* material, int shaderID)
	{
		glDeleteTextures(1, &material->albedoMap->ID);
		glDeleteTextures(1, &material->metallicRoughnessMap->ID);
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
				glBufferSubData(GL_ARRAY_BUFFER, 0, bufferView->byteLength, bufferViewData);
			}
			else if (bufferView->target == mitevox::BufferView::ELEMENT_ARRAY_BUFFER)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferView->ID);
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

	void uploadMesh(mitevox::Mesh* mesh, int shaderID)
	{
		if (!shaders[shaderID]->use())
			return;

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
							attributeAccessor->getComponentsCount(),
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
							attributeAccessor->getComponentsCount(),
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

			uploadMaterial(meshPrimitive->material, shaderID);

			PRINT_RENDERER_ERRORS;
		}

		checkMeshBuffers(mesh);
	}

	void updateMesh(mitevox::Mesh* mesh, int shaderID)
	{
		if (!shaders[shaderID]->use())
			return;

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

			//updateMaterial(meshPrimitive->material, shaderID);

			PRINT_RENDERER_ERRORS;
		}
	}

	void removeMesh(mitevox::Mesh* mesh, int shaderID)
	{
		if (!shaders[shaderID]->use())
			return;

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
	}

	void renderMesh(
		RendererSettings* renderer,
		int shaderID,
		mitevox::Mesh* mesh,
		mathem::GeometryTransform* transform,
		Camera* camera,
		mathem::Transform* cameraTransform)
	{
		if (!shaders[shaderID]->use())
			return;

		checkMeshBuffers(mesh);

		shaders[shaderID]->setInt("hasCubemap", 0);
		shaders[shaderID]->setVec3("viewPos", cameraTransform->x, cameraTransform->y, cameraTransform->z);

		/*const glm::highp_quat rotation(
			transform->rotation.binary.scalar, 
			transform->rotation.binary.vector.x(), 
			transform->rotation.binary.vector.y(), 
			transform->rotation.binary.vector.z());
		glm::mat4 rotationMatrix = glm::mat4_cast<float, glm::packed_highp>(rotation);
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(transform->translation.x(), transform->translation.y(), transform->translation.z()));
		modelMatrix *= rotationMatrix;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(transform->scale.x(), transform->scale.y(), transform->scale.z()));*/

		mathem::Matrix4x4 modelMatrix = mathem::transformToMatrix(*transform);
		shaders[shaderID]->setMatrix4x4("modelMatrix", modelMatrix);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 viewProjectionMatrix = projection * view;
		shaders[shaderID]->setMat4("viewProjectionMatrix", viewProjectionMatrix);

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

	void selectSkybox(Skybox* skybox)
	{
		glBindVertexArray(skybox->vertexID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemap->textureID);
	}

	void removeSkybox(Skybox* skybox)
	{
		glDeleteBuffers(1, &skybox->vertexID);
		glDeleteTextures(1, &skybox->cubemap->textureID);
	}

	void renderSkybox(RendererSettings* renderer, Skybox* skybox, render::Camera* camera, mathem::Transform* cameraTransform)
	{
		if (!shaders[skybox->shaderID]->use())
			return;

		renderer->amountOfDrawCalls++;

		selectSkybox(skybox);

		GLenum error = glGetError();

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));
		view = glm::mat4(glm::mat3(view));

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 positionTransform = glm::mat4(1.0f);
		positionTransform = proj * view;

		shaders[skybox->shaderID]->setMat4("positionTransform", positionTransform);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}
