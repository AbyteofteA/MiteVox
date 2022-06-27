
#include <vector>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "API_ModelFunctions.h"

#include "engine/MiteVox/src/Material/Material.h"
#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"

#define RENDERER_VERTEX_SIZE 8 // Amount of floats to store a vertex (XYZ + UV + IJK)
#define RENDERER_TRIANGLE_SIZE RENDERER_VERTEX_SIZE * 3

namespace render
{
	float* modelToXYZUVIJK(Mesh3D* mesh)
	{
		size_t modelDumpSize = (size_t)sizeof(float) * mesh->polygonsCount * RENDERER_TRIANGLE_SIZE;
		float* result = (float*)malloc((size_t)modelDumpSize);

		for (size_t i = 0; i < mesh->polygonsCount; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				size_t offset = i * RENDERER_TRIANGLE_SIZE + j * RENDERER_VERTEX_SIZE;

				// X Y Z
				size_t positionIndex = mesh->polygons[i].positions[j] - 1;
				mathem::Point3D position = mesh->positions[positionIndex];
				result[offset + 0] = position.x;
				result[offset + 1] = position.y;
				result[offset + 2] = position.z;

				// U V
				mathem::Point3D textureCoord;
				if (mesh->polygons[i].textureCoords != nullptr)
				{
					size_t textureCoordIndex = mesh->polygons[i].textureCoords[j] - 1;
					textureCoord = mesh->textureCoords[textureCoordIndex];
					result[offset + 3] = textureCoord.x;
					result[offset + 4] = textureCoord.y;
				}

				// I J K
				mathem::Vector3D normal;
				if (mesh->polygons[i].normals != nullptr)
				{
					size_t normalIndex = mesh->polygons[i].normals[j] - 1;
					normal = mesh->normals[normalIndex];
					result[offset + 5] = normal.x();
					result[offset + 6] = normal.y();
					result[offset + 7] = normal.z();
				}
			}
		}

		return result;
	}

	// Model3D

	void uploadModel3D(Model3D* model3D)
	{
		if (!shaders[model3D->shaderID]->use())
			return;

		if (model3D->mesh->isUploaded == 0)
		{
			float* modelData = modelToXYZUVIJK(model3D->mesh);

			unsigned int modelVBO;
			glGenBuffers(1, &modelVBO);
			glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(float) * RENDERER_TRIANGLE_SIZE * model3D->mesh->polygonsCount,
				modelData,
				GL_STATIC_DRAW);

			glGenVertexArrays(1, &model3D->mesh->vertexBufferID);
			glBindVertexArray(model3D->mesh->vertexBufferID);
			
			GLint posAttrib = glGetAttribLocation(shaders[model3D->shaderID]->textureID, "position");
			GLint texAttrib = glGetAttribLocation(shaders[model3D->shaderID]->textureID, "texcoord");
			GLint normalAttrib = glGetAttribLocation(shaders[model3D->shaderID]->textureID, "normal");

			glEnableVertexAttribArray(posAttrib);
			glEnableVertexAttribArray(texAttrib);
			glEnableVertexAttribArray(normalAttrib);
			glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE * sizeof(GLfloat), 0);
			glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
			glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

			model3D->mesh->isUploaded = 1;

			free(modelData);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		uploadMaterial(model3D->material, model3D->shaderID);
	}

	void selectModel3D(Model3D* model3D)
	{
		if (!shaders[model3D->shaderID]->use())
			return;

		glBindVertexArray(model3D->mesh->vertexBufferID);
		selectMaterial(model3D->material, model3D->shaderID);
	}

	void removeModel3D(Model3D* model3D)
	{
		if (!shaders[model3D->shaderID]->use())
			return;

		glDeleteBuffers(1, &model3D->mesh->vertexBufferID);
		removeMaterial(model3D->material, model3D->shaderID);

		model3D->mesh->isUploaded = 0;
	}


	// Scene

	extern float skyboxVert[];

	void uploadSkybox(Skybox* skybox)
	{
		GLint posAttrib = glGetAttribLocation(shaders[1]->textureID, "position");

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
				glUniform1i(glGetUniformLocation(shaders[shaderID]->textureID, "albedoMap"), textureUnit);
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
				glUniform1i(glGetUniformLocation(shaders[shaderID]->textureID, "metallicRoughnessMap"), textureUnit);
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

			if (auto positionAccessor = meshPrimitive->getPositions())
			{
				checkBufferView(positionAccessor->bufferView);
			}
			if (auto textureAccessor = meshPrimitive->getTextureCoords_0())
			{
				checkBufferView(textureAccessor->bufferView);
			}
			if (auto normalAccessor = meshPrimitive->getNormals())
			{
				checkBufferView(normalAccessor->bufferView);
			}
			if (auto indexAccessor = meshPrimitive->getIndeces())
			{
				checkBufferView(indexAccessor->bufferView);
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void uploadMesh(mitevox::Mesh* mesh, int shaderID)
	{
		if (!shaders[shaderID]->use())
			return;

		GLint posAttrib = glGetAttribLocation(shaders[shaderID]->textureID, "position");
		GLint texAttrib = glGetAttribLocation(shaders[shaderID]->textureID, "texcoord");
		GLint normalAttrib = glGetAttribLocation(shaders[shaderID]->textureID, "normal");

		int64_t meshesPrimitivesCount = mesh->primitives.getElementsCount();
		for (int64_t primitiveIndex = 0; primitiveIndex < meshesPrimitivesCount; ++primitiveIndex)
		{
			mitevox::MeshPrimitive* meshPrimitive = mesh->primitives.getElement(primitiveIndex);

			if (auto positionAccessor = meshPrimitive->getPositions())
			{
				uploadBufferView(positionAccessor->bufferView);
			}
			if (auto textureAccessor = meshPrimitive->getTextureCoords_0())
			{
				uploadBufferView(textureAccessor->bufferView);
			}
			if (auto normalAccessor = meshPrimitive->getNormals())
			{
				uploadBufferView(normalAccessor->bufferView);
			}
			if (auto indexAccessor = meshPrimitive->getIndeces())
			{
				uploadBufferView(indexAccessor->bufferView);
			}

			glGenVertexArrays(1, &meshPrimitive->ID);
			glBindVertexArray(meshPrimitive->ID);

			if (auto positionAccessor = meshPrimitive->getPositions())
			{
				glEnableVertexAttribArray(posAttrib);
				glBindBuffer(GL_ARRAY_BUFFER, positionAccessor->bufferView->ID);
				glVertexAttribPointer(posAttrib, 3, 
					positionAccessor->componentType, 
					positionAccessor->normalized,
					positionAccessor->bufferView->byteStride,
					(void*)positionAccessor->byteOffset);
			}
			if (auto textureAccessor = meshPrimitive->getTextureCoords_0())
			{
				glEnableVertexAttribArray(texAttrib);
				glBindBuffer(GL_ARRAY_BUFFER, textureAccessor->bufferView->ID);
				glVertexAttribPointer(texAttrib, 2, 
					textureAccessor->componentType, 
					textureAccessor->normalized,
					textureAccessor->bufferView->byteStride,
					(void*)textureAccessor->byteOffset);
			}
			if (auto normalAccessor = meshPrimitive->getNormals())
			{
				glEnableVertexAttribArray(normalAttrib);
				glBindBuffer(GL_ARRAY_BUFFER, normalAccessor->bufferView->ID);
				glVertexAttribPointer(
					normalAttrib, 3, 
					normalAccessor->componentType, 
					normalAccessor->normalized,
					normalAccessor->bufferView->byteStride,
					(void*)normalAccessor->byteOffset);
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

			if (auto positionAccessor = meshPrimitive->getPositions())
			{
				updateBufferView(positionAccessor->bufferView);
			}
			if (auto textureAccessor = meshPrimitive->getTextureCoords_0())
			{
				updateBufferView(textureAccessor->bufferView);
			}
			if (auto normalAccessor = meshPrimitive->getNormals())
			{
				updateBufferView(normalAccessor->bufferView);
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

			if (auto positionAccessor = meshPrimitive->getPositions())
			{
				removeBufferView(positionAccessor->bufferView);
			}
			if (auto textureAccessor = meshPrimitive->getTextureCoords_0())
			{
				removeBufferView(textureAccessor->bufferView);
			}
			if (auto normalsAccessor = meshPrimitive->getNormals())
			{
				removeBufferView(normalsAccessor->bufferView);
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

		const glm::highp_quat rotation(transform->rotation.s(), transform->rotation.x(), transform->rotation.y(), transform->rotation.z());
		glm::mat4 global = glm::mat4_cast<float, glm::packed_highp>(rotation);
		global = glm::translate(global, glm::vec3(transform->translation.x(), transform->translation.y(), transform->translation.z()));
		global = glm::scale(global, glm::vec3(transform->scale.x(), transform->scale.y(), transform->scale.z()));
		//global = glm::rotate(global, glm::radians(transform->rotation.x()), glm::vec3(1.0, 0.0, 0.0));
		//global = glm::rotate(global, glm::radians(transform->rotation.y()), glm::vec3(0.0, 1.0, 0.0));
		//global = glm::rotate(global, glm::radians(transform->rotation.z()), glm::vec3(0.0, 0.0, 1.0));

		shaders[shaderID]->setMat4("model", global);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 positionTransform = proj * view * global;
		shaders[shaderID]->setMat4("positionTransform", positionTransform);

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
				glDrawArrays(GL_TRIANGLES, 0, meshPrimitive->attributes.positionAccessor->count);
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			PRINT_RENDERER_ERRORS;
		}
	}

	void uploadNodeRecursively(mitevox::Node* node, int shaderID)
	{
		if (mitevox::Mesh* meshToRender = node->getMeshToRender())
		{
			render::uploadMesh(meshToRender, shaderID);
		}

		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			uploadNodeRecursively(node->children.getElement(i), shaderID);
		}
	}

	void removeNodeRecursively(mitevox::Node* node, int shaderID)
	{
		if (mitevox::Mesh* meshToRender = node->getMeshToRender())
		{
			render::removeMesh(meshToRender, shaderID);
		}

		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			removeNodeRecursively(node->children.getElement(i), shaderID);
		}
	}

	void renderNodeRecursively(RendererSettings* renderer, int shaderID, mitevox::Node* node, mathem::GeometryTransform* transform, Camera* camera, mathem::Transform* cameraTransform)
	{
		mathem::GeometryTransform nodeGlobalTransform = *transform * node->transform;
		if (mitevox::Mesh* meshToRender = node->getMeshToRender())
		{
			render::renderMesh(renderer, shaderID, meshToRender, &nodeGlobalTransform, camera, cameraTransform);
		}

		size_t childrenCount = node->children.getElementsCount();
		for (size_t i = 0; i < childrenCount; ++i)
		{
			renderNodeRecursively(
				renderer, 
				shaderID, 
				node->children.getElement(i), 
				&nodeGlobalTransform, 
				camera, 
				cameraTransform);
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


	// Drawing

	void renderModel3D(RendererSettings* renderer, Model3D* model3D, mathem::Transform* transform, render::Camera* camera, mathem::Transform* cameraTransform)
	{
		if (!shaders[model3D->shaderID]->use())
			return;

		renderer->amountOfDrawCalls++;

		shaders[model3D->shaderID]->setInt("hasCubemap", 0);
		shaders[model3D->shaderID]->setVec3("viewPos", cameraTransform->x, cameraTransform->y, cameraTransform->z);

		glm::mat4 global = glm::mat4(1.0f);
		global = glm::translate(global, glm::vec3(transform->x, transform->y, transform->z));
		global = glm::scale(global, glm::vec3(transform->scaleX, transform->scaleY, transform->scaleZ));
		global = glm::rotate(global, glm::radians(transform->angleX), glm::vec3(1.0, 0.0, 0.0));
		global = glm::rotate(global, glm::radians(transform->angleY), glm::vec3(0.0, 1.0, 0.0));
		global = glm::rotate(global, glm::radians(transform->angleZ), glm::vec3(0.0, 0.0, 1.0));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(model3D->transform.x, model3D->transform.y, model3D->transform.z));
		model = glm::scale(model, glm::vec3(model3D->transform.scaleX, model3D->transform.scaleY, model3D->transform.scaleZ));
		model = glm::rotate(model, glm::radians(model3D->transform.angleX), glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(model3D->transform.angleY), glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, glm::radians(model3D->transform.angleZ), glm::vec3(0.0, 0.0, 1.0));

		glm::mat4 modelTransform = glm::mat4(1.0f);
		modelTransform = global * model;
		shaders[model3D->shaderID]->setMat4("model", modelTransform);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 positionTransform = glm::mat4(1.0f);
		positionTransform = proj * view * modelTransform;
		shaders[model3D->shaderID]->setMat4("positionTransform", positionTransform);
		
		selectModel3D(model3D);

		glDrawArrays(GL_TRIANGLES, 0, model3D->mesh->polygonsCount * 3);
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
