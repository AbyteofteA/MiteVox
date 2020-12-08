
#include <vector>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "ModelFunctions.h"

namespace render
{
	float* modelToXYZUVIJK(Mesh3D* mesh)
	{
		unsigned long long size = (unsigned long long)sizeof(float) * mesh->amOfFaces * 24;
		float* result = (float*)malloc((size_t)size);

		for (unsigned long long i = 0; i < mesh->amOfFaces; i++)
		{
			for (unsigned long long j = 0; j < 3; j++)
			{
				// X Y Z
				result[i * 24 + j * 8 + 0] = mesh->v[mesh->f[i].p[j] - 1].x;
				result[i * 24 + j * 8 + 1] = mesh->v[mesh->f[i].p[j] - 1].y;
				result[i * 24 + j * 8 + 2] = mesh->v[mesh->f[i].p[j] - 1].z;

				// U V
				if (mesh->f[i].t != nullptr)
				{
					result[i * 24 + j * 8 + 3] = mesh->vt[mesh->f[i].t[j] - 1].x;
					result[i * 24 + j * 8 + 4] = mesh->vt[mesh->f[i].t[j] - 1].y;
				}

				// I J K
				if (mesh->vn != nullptr)
				{
					result[i * 24 + j * 8 + 5] = mesh->vn[mesh->f[i].n[j] - 1].i;
					result[i * 24 + j * 8 + 6] = mesh->vn[mesh->f[i].n[j] - 1].j;
					result[i * 24 + j * 8 + 7] = mesh->vn[mesh->f[i].n[j] - 1].k;
				}
			}
		}

		return result;
	}

	// Model3D

	void uploadModel3D(Model3D* model3D)
	{
		if (!standardShaders[model3D->shaderID]->use())
			return;

		if (model3D->mesh->isUploaded == 0)
		{
			float* modelData = modelToXYZUVIJK(model3D->mesh);

			GLint posAttrib = glGetAttribLocation(standardShaders[model3D->shaderID]->textureID, "position");
			GLint texAttrib = glGetAttribLocation(standardShaders[model3D->shaderID]->textureID, "texcoord");
			GLint normalAttrib = glGetAttribLocation(standardShaders[model3D->shaderID]->textureID, "normal");
			

			unsigned int modelVBO;
			glGenVertexArrays(1, &model3D->mesh->vertexBufferID);
			glGenBuffers(1, &modelVBO);
			glBindVertexArray(model3D->mesh->vertexBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24 * model3D->mesh->amOfFaces, modelData, GL_STATIC_DRAW);

			glEnableVertexAttribArray(posAttrib);
			glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(texAttrib);
			glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(normalAttrib);
			glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

			model3D->mesh->isUploaded = 1;

			free(modelData);
		}

		if (model3D->material->isUploaded == 0)
		{
			unsigned char* image;
			image = (unsigned char*)model3D->material->albedoMap.imageData;
			glGenTextures(1, &model3D->material->ambientMapID);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, model3D->material->ambientMapID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, model3D->material->albedoMap.width, model3D->material->albedoMap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glUniform1i(glGetUniformLocation(standardShaders[model3D->shaderID]->textureID, "albedoMap"), 0);
			
			image = (unsigned char*)model3D->material->metallicMap.imageData;
			glGenTextures(1, &model3D->material->specularMapID);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, model3D->material->specularMapID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, model3D->material->metallicMap.width, model3D->material->metallicMap.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glUniform1i(glGetUniformLocation(standardShaders[model3D->shaderID]->textureID, "metallicMap"), 1);
			

			model3D->material->isUploaded = 1;
		}
	}
	void selectModel3D(Model3D* model3D, ShaderOpenGL* shader)
	{
		glBindVertexArray(model3D->mesh->vertexBufferID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model3D->material->ambientMapID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, model3D->material->specularMapID);
	}
	void removeModel3D(Model3D* model3D)
	{
		glDeleteBuffers(1, &model3D->mesh->vertexBufferID);
		glDeleteTextures(1, &model3D->material->ambientMapID);
		glDeleteTextures(1, &model3D->material->specularMapID);

		model3D->mesh->isUploaded = 0;
		model3D->material->isUploaded = 0;
	}


	// Scene

	extern float skyboxVert[];

	void uploadSkybox(Skybox* skybox)
	{
		GLint posAttrib = glGetAttribLocation(standardShaders[1]->textureID, "position");

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

	void renderModel3D(Model3D* model3D, mathem::Transform* transform, render::Camera* camera, mathem::Transform* cameraTransform)
	{
		if (!standardShaders[model3D->shaderID]->use())
			return;

		selectModel3D(model3D, standardShaders[model3D->shaderID]);

		standardShaders[model3D->shaderID]->setInt("hasCubemap", 0);
		standardShaders[model3D->shaderID]->setVec3("viewPos", cameraTransform->x, cameraTransform->y, cameraTransform->z);

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
		standardShaders[model3D->shaderID]->setMat4("model", modelTransform);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 positionTransform = glm::mat4(1.0f);
		positionTransform = proj * view * modelTransform;
		standardShaders[model3D->shaderID]->setMat4("positionTransform", positionTransform);

		// Pass the material to a shader.
		glm::vec3 tmp = { model3D->material->albedo.r, model3D->material->albedo.g, model3D->material->albedo.b };
		standardShaders[model3D->shaderID]->setVec3("material.albedo", tmp);
		tmp = { model3D->material->roughness.r, model3D->material->roughness.g, model3D->material->roughness.b };
		standardShaders[model3D->shaderID]->setVec3("material.roughness", tmp);
		tmp = { model3D->material->metallicity.r, model3D->material->metallicity.g, model3D->material->metallicity.b };
		standardShaders[model3D->shaderID]->setVec3("material.metallicity", tmp);
		standardShaders[model3D->shaderID]->setFloat("material.specularExponent", model3D->material->specularExponent);

		glDrawArrays(GL_TRIANGLES, 0, model3D->mesh->amOfFaces * 24);
	}

	void renderSkybox(Skybox* skybox, render::Camera* camera, mathem::Transform* cameraTransform)
	{
		if (!standardShaders[skybox->shaderID]->use())
			return;

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

		standardShaders[skybox->shaderID]->setMat4("positionTransform", positionTransform);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

}
