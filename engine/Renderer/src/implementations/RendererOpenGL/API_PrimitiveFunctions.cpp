
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "API_PrimitiveFunctions.h"

namespace render
{
	
	void drawPoint(RendererSettings* renderer, Point point)
	{
		unsigned int size = renderer->points.getSizeData();
		renderer->points.setSizeData(size + 1);
		renderer->points.data[size] = point;
	}
	void drawLine(RendererSettings* renderer, Point point1, Point point2)
	{
		unsigned int size = renderer->lines.getSizeData();
		renderer->lines.setSizeData(size + 2);
		renderer->lines.data[size] = point1;
		renderer->lines.data[size + 1] = point2;
	}
	void drawTriangle(RendererSettings* renderer, Point point1, Point point2, Point point3)
	{
		unsigned int size = renderer->triangles.getSizeData();
		renderer->triangles.setSizeData(size + 3);
		renderer->triangles.data[size] = point1;
		renderer->triangles.data[size + 1] = point2;
		renderer->triangles.data[size + 2] = point3;
	}

	void drawCross(RendererSettings* renderer, Point point, float size)
	{
		render::Point point1 = point;
		render::Point point2 = point;
		render::Point point3 = point;
		render::Point point4 = point;
		render::Point point5 = point;
		render::Point point6 = point;

		point1.position.x += size;
		point2.position.y += size;
		point3.position.z += size;
		point4.position.x += -size;
		point5.position.y += -size;
		point6.position.z += -size;

		render::drawLine(renderer, point1, point4);
		render::drawLine(renderer, point2, point5);
		render::drawLine(renderer, point3, point6);
	}
	void drawSnowflake(RendererSettings* renderer, Point point, float size)
	{
		drawCross(renderer, point, size);

		size /= 2;

		render::Point point1 = { {size + point.position.x, size + point.position.y, size + point.position.z}, point.color};
		render::Point point2 = { {size + point.position.x, -size + point.position.y, size + point.position.z},  point.color };
		render::Point point3 = { {size + point.position.x, -size + point.position.y, -size + point.position.z},  point.color };
		render::Point point4 = { {size + point.position.x, size + point.position.y, -size + point.position.z},  point.color };
		render::Point point5 = { {-size + point.position.x, size + point.position.y, -size + point.position.z},  point.color };
		render::Point point6 = { {-size + point.position.x, -size + point.position.y, -size + point.position.z},  point.color };
		render::Point point7 = { {-size + point.position.x, -size + point.position.y, size + point.position.z},  point.color };
		render::Point point8 = { {-size + point.position.x, size + point.position.y, size + point.position.z},  point.color };

		render::drawLine(renderer, point1, point6);
		render::drawLine(renderer, point2, point5);
		render::drawLine(renderer, point3, point8);
		render::drawLine(renderer, point4, point7);
	}

	void renderPoints(RendererSettings* renderer, Camera* camera, mathem::Transform* cameraTransform)
	{
		unsigned int shaderIndex = renderer->primitiveShaderID;
		if (!shaders[shaderIndex]->use())
			return;

		unsigned int amountOfPoints = renderer->points.getSizeData();
		if (amountOfPoints <= 0)
			return;

		renderer->amountOfDrawCalls++;

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 positionTransform = glm::mat4(1.0f);
		positionTransform = proj * view;
		shaders[shaderIndex]->setMat4("positionTransform", positionTransform);

		GLint posAttrib = glGetAttribLocation(shaders[shaderIndex]->textureID, "position");
		GLint colorAttrib = glGetAttribLocation(shaders[shaderIndex]->textureID, "color");

		unsigned int pointsVBO;
		unsigned int pointsArrayID;
		glGenVertexArrays(1, &pointsArrayID);
		glGenBuffers(1, &pointsVBO);
		glBindVertexArray(pointsArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * amountOfPoints,
			renderer->points.data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		glDrawArrays(GL_POINTS, 0, amountOfPoints * 7);

		glDeleteBuffers(1, &pointsArrayID);
		renderer->points.setSizeData(0);
	}

	void renderLines(RendererSettings* renderer, Camera* camera, mathem::Transform* cameraTransform)
	{
		unsigned int shaderIndex = renderer->primitiveShaderID;
		if (!shaders[shaderIndex]->use())
			return;

		unsigned int amountOfLines = renderer->lines.getSizeData();
		if (amountOfLines <= 0)
			return;

		renderer->amountOfDrawCalls++;

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 positionTransform = glm::mat4(1.0f);
		positionTransform = proj * view;
		shaders[shaderIndex]->setMat4("positionTransform", positionTransform);

		GLint posAttrib = glGetAttribLocation(shaders[shaderIndex]->textureID, "position");
		GLint colorAttrib = glGetAttribLocation(shaders[shaderIndex]->textureID, "color");

		unsigned int linesVBO;
		unsigned int linesArrayID;
		glGenVertexArrays(1, &linesArrayID);
		glGenBuffers(1, &linesVBO);
		glBindVertexArray(linesArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * amountOfLines,
			renderer->lines.data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		glDrawArrays(GL_LINES, 0, amountOfLines * 14);

		glDeleteBuffers(1, &linesArrayID);
		renderer->lines.setSizeData(0);
	}

	void renderTriangles(RendererSettings* renderer, Camera* camera, mathem::Transform* cameraTransform)
	{
		unsigned int shaderIndex = renderer->primitiveShaderID;
		if (!shaders[shaderIndex]->use())
			return;

		unsigned int amountOfTriangles = renderer->triangles.getSizeData();
		if (amountOfTriangles <= 0)
			return;

		renderer->amountOfDrawCalls++;

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 positionTransform = glm::mat4(1.0f);
		positionTransform = proj * view;
		shaders[shaderIndex]->setMat4("positionTransform", positionTransform);

		GLint posAttrib = glGetAttribLocation(shaders[shaderIndex]->textureID, "position");
		GLint colorAttrib = glGetAttribLocation(shaders[shaderIndex]->textureID, "color");

		unsigned int trianglesVBO;
		unsigned int trianglesArrayID;
		glGenVertexArrays(1, &trianglesArrayID);
		glGenBuffers(1, &trianglesVBO);
		glBindVertexArray(trianglesArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, trianglesVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * amountOfTriangles,
			renderer->triangles.data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, amountOfTriangles * 21);
		if (renderer->backfaceCulling)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		glDeleteBuffers(1, &trianglesArrayID);
		renderer->triangles.setSizeData(0);
	}
}