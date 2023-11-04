#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <utility>

namespace render
{
	void drawPoint(RendererSettings* renderer, Point point)
	{
		renderer->points.appendElement(point);
	}

	void drawPoint(RendererSettings* renderer, mathem::Vector3D position, ColorRGBAf color)
	{
		Point point;
		point.position = position;
		point.color = color;
		renderer->points.appendElement(point);
	}

	void drawLine(RendererSettings* renderer, Point point1, Point point2)
	{
		renderer->lines.appendElement(point1);
		renderer->lines.appendElement(point2);
	}
	void drawTriangle(RendererSettings* renderer, Point point1, Point point2, Point point3)
	{
		renderer->triangles.appendElement(point1);
		renderer->triangles.appendElement(point2);
		renderer->triangles.appendElement(point3);
	}

	void drawCross(RendererSettings* renderer, Point point, float size)
	{
		render::Point point1 = point;
		render::Point point2 = point;
		render::Point point3 = point;
		render::Point point4 = point;
		render::Point point5 = point;
		render::Point point6 = point;

		point1.position.x() += size;
		point2.position.y() += size;
		point3.position.z() += size;
		point4.position.x() += -size;
		point5.position.y() += -size;
		point6.position.z() += -size;

		render::drawLine(renderer, point1, point4);
		render::drawLine(renderer, point2, point5);
		render::drawLine(renderer, point3, point6);
	}

	void drawCross(RendererSettings* renderer, mathem::Vector3D position, ColorRGBAf color, float size)
	{
		Point point;
		point.position = position;
		point.color = color;
		drawCross(renderer, point, size);
	}

	void drawArrow(RendererSettings* renderer, mathem::Vector3D origin, mathem::Vector3D direction, ColorRGBAf color)
	{
		render::Point pointOrigin;
		pointOrigin.position = origin;
		pointOrigin.color = color;
		drawSnowflake(renderer, pointOrigin, 0.05f);

		render::Point normalPoint1, normalPoint2;
		normalPoint1.position = origin;
		normalPoint1.color = color;
		normalPoint2.position = origin + direction;
		normalPoint2.color = color;
		drawLine(renderer, normalPoint1, normalPoint2);

		mathem::Vector3D pointArrowDelta = direction * 0.2f;
		render::Point pointArrow = normalPoint2;
		pointArrow.position -= pointArrowDelta;

		drawSnowflake(renderer, pointArrow, 0.1f);
	}

	void drawSnowflake(RendererSettings* renderer, Point point, float size)
	{
		drawCross(renderer, point, size);

		size /= 2;

		render::Point point1 = { {size + point.position.x(), size + point.position.y(), size + point.position.z()}, point.color};
		render::Point point2 = { {size + point.position.x(), -size + point.position.y(), size + point.position.z()},  point.color };
		render::Point point3 = { {size + point.position.x(), -size + point.position.y(), -size + point.position.z()},  point.color };
		render::Point point4 = { {size + point.position.x(), size + point.position.y(), -size + point.position.z()},  point.color };
		render::Point point5 = { {-size + point.position.x(), size + point.position.y(), -size + point.position.z()},  point.color };
		render::Point point6 = { {-size + point.position.x(), -size + point.position.y(), -size + point.position.z()},  point.color };
		render::Point point7 = { {-size + point.position.x(), -size + point.position.y(), size + point.position.z()},  point.color };
		render::Point point8 = { {-size + point.position.x(), size + point.position.y(), size + point.position.z()},  point.color };

		render::drawLine(renderer, point1, point6);
		render::drawLine(renderer, point2, point5);
		render::drawLine(renderer, point3, point8);
		render::drawLine(renderer, point4, point7);
	}

	void drawSnowflake(RendererSettings* renderer, mathem::Vector3D position, ColorRGBAf color, float size)
	{
		Point point;
		point.position = position;
		point.color = color;
		drawSnowflake(renderer, point, size);
	}

	void renderPoints(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform)
	{
		unsigned int shaderIndex = renderer->primitiveShaderID;
		if (!shaders[shaderIndex]->use())
			return;

		size_t amountOfPoints = renderer->points.getElementsCount();
		if (amountOfPoints == 0)
			return;

		renderer->amountOfDrawCalls++;

		glm::mat4 viewMatrix = camera->getViewMatrix(cameraTransform);
		glm::mat4 projectionMatrix = camera->getProjectionMatrix();
		shaders[shaderIndex]->setMat4("viewProjectionMatrix", projectionMatrix * viewMatrix);

		GLint posAttrib = glGetAttribLocation(shaders[shaderIndex]->shaderID, "position");
		GLint colorAttrib = glGetAttribLocation(shaders[shaderIndex]->shaderID, "color");

		unsigned int pointsVBO;
		glGenBuffers(1, &pointsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * amountOfPoints,
		renderer->points.getElementsArray(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		unsigned int pointsArrayID;
		glGenVertexArrays(1, &pointsArrayID);
		glBindVertexArray(pointsArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		glDrawArrays(GL_POINTS, 0, amountOfPoints * 7);

		glDeleteVertexArrays(1, &pointsArrayID);
		glDeleteBuffers(1, &pointsVBO);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		renderer->points.clear();
	}

	void renderLines(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform)
	{
		unsigned int shaderIndex = renderer->primitiveShaderID;
		if (!shaders[shaderIndex]->use())
			return;

		size_t amountOfLines = renderer->lines.getElementsCount();
		if (amountOfLines == 0)
			return;

		renderer->amountOfDrawCalls++;

		glm::mat4 viewMatrix = camera->getViewMatrix(cameraTransform);
		glm::mat4 projectionMatrix = camera->getProjectionMatrix();
		shaders[shaderIndex]->setMat4("viewProjectionMatrix", projectionMatrix * viewMatrix);

		GLint posAttrib = glGetAttribLocation(shaders[shaderIndex]->shaderID, "position");
		GLint colorAttrib = glGetAttribLocation(shaders[shaderIndex]->shaderID, "color");

		unsigned int linesVBO;
		glGenBuffers(1, &linesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * amountOfLines,
		renderer->lines.getElementsArray(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		unsigned int linesArrayID;
		glGenVertexArrays(1, &linesArrayID);
		glBindVertexArray(linesArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		glDrawArrays(GL_LINES, 0, amountOfLines * 14);

		glDeleteVertexArrays(1, &linesArrayID);
		glDeleteBuffers(1, &linesVBO);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		renderer->lines.clear();
	}

	void renderTriangles(RendererSettings* renderer, Camera* camera, mathem::GeometryTransform* cameraTransform)
	{
		unsigned int shaderIndex = renderer->primitiveShaderID;
		if (!shaders[shaderIndex]->use())
			return;

		size_t amountOfTriangles = renderer->triangles.getElementsCount();
		if (amountOfTriangles == 0)
			return;

		renderer->amountOfDrawCalls++;

		glm::mat4 viewMatrix = camera->getViewMatrix(cameraTransform);
		glm::mat4 projectionMatrix = camera->getProjectionMatrix();
		shaders[shaderIndex]->setMat4("viewProjectionMatrix", projectionMatrix * viewMatrix);

		GLint posAttrib = glGetAttribLocation(shaders[shaderIndex]->shaderID, "position");
		GLint colorAttrib = glGetAttribLocation(shaders[shaderIndex]->shaderID, "color");

		unsigned int trianglesVBO;
		glGenBuffers(1, &trianglesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, trianglesVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * amountOfTriangles,
		renderer->triangles.getElementsArray(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		unsigned int trianglesArrayID;
		glGenVertexArrays(1, &trianglesArrayID);
		glBindVertexArray(trianglesArrayID);

		glBindBuffer(GL_ARRAY_BUFFER, trianglesVBO);
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

		glDeleteVertexArrays(1, &trianglesArrayID);
		glDeleteBuffers(1, &trianglesVBO);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		renderer->triangles.clear();
	}
}