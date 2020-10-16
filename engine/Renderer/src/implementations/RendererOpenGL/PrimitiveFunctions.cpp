
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "PrimitiveFunctions.h"

namespace render
{
	
	void drawPoint(Point point)
	{
		unsigned int size = RendererSettingsDebug.points.getSizeData();
		RendererSettingsDebug.points.setSizeData(size + 1);
		RendererSettingsDebug.points.data[size] = point;
	}
	void drawLine(Point point1, Point point2)
	{
		unsigned int size = RendererSettingsDebug.lines.getSizeData();
		RendererSettingsDebug.lines.setSizeData(size + 2);
		RendererSettingsDebug.lines.data[size] = point1;
		RendererSettingsDebug.lines.data[size + 1] = point2;
	}
	void drawTriangle(Point point1, Point point2, Point point3)
	{
		unsigned int size = RendererSettingsDebug.triangles.getSizeData();
		RendererSettingsDebug.triangles.setSizeData(size + 3);
		RendererSettingsDebug.triangles.data[size] = point1;
		RendererSettingsDebug.triangles.data[size + 1] = point2;
		RendererSettingsDebug.triangles.data[size + 2] = point3;
	}

	void drawCross(Point point, float size)
	{
		render::Point point1 = point;
		render::Point point2 = point;
		render::Point point3 = point;
		render::Point point4 = point;
		render::Point point5 = point;
		render::Point point6 = point;

		point1.pos.x += size;
		point2.pos.y += size;
		point3.pos.z += size;
		point4.pos.x += -size;
		point5.pos.y += -size;
		point6.pos.z += -size;

		render::drawLine(point1, point4);
		render::drawLine(point2, point5);
		render::drawLine(point3, point6);
	}


	void drawSnowflake(Point point, float size)
	{
		drawCross(point, size);

		size /= 2;

		render::Point point1 = { {size + point.pos.x, size + point.pos.y, size + point.pos.z}, point.color};
		render::Point point2 = { {size + point.pos.x, -size + point.pos.y, size + point.pos.z},  point.color };
		render::Point point3 = { {size + point.pos.x, -size + point.pos.y, -size + point.pos.z},  point.color };
		render::Point point4 = { {size + point.pos.x, size + point.pos.y, -size + point.pos.z},  point.color };
		render::Point point5 = { {-size + point.pos.x, size + point.pos.y, -size + point.pos.z},  point.color };
		render::Point point6 = { {-size + point.pos.x, -size + point.pos.y, -size + point.pos.z},  point.color };
		render::Point point7 = { {-size + point.pos.x, -size + point.pos.y, size + point.pos.z},  point.color };
		render::Point point8 = { {-size + point.pos.x, size + point.pos.y, size + point.pos.z},  point.color };

		render::drawLine(point1, point6);
		render::drawLine(point2, point5);
		render::drawLine(point3, point8);
		render::drawLine(point4, point7);
	}


	void renderPoints(render::Camera* camera, mathem::Transform* cameraTransform)
	{
		unsigned int shaderIndex = 2;
		if (!standardShaders[shaderIndex]->use())
			return;

		unsigned int amountOfPoints = RendererSettingsDebug.points.getSizeData();
		if (amountOfPoints <= 0)
			return;

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 positionTransform = glm::mat4(1.0f);
		positionTransform = proj * view;
		standardShaders[shaderIndex]->setMat4("positionTransform", positionTransform);

		GLint posAttrib = glGetAttribLocation(standardShaders[shaderIndex]->textureID, "position");
		GLint colorAttrib = glGetAttribLocation(standardShaders[shaderIndex]->textureID, "color");

		unsigned int pointsVBO;
		unsigned int pointsArrayID;
		glGenVertexArrays(1, &pointsArrayID);
		glGenBuffers(1, &pointsVBO);
		glBindVertexArray(pointsArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * amountOfPoints,
			RendererSettingsDebug.points.data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		glDrawArrays(GL_POINTS, 0, amountOfPoints * 7);

		glDeleteBuffers(1, &pointsArrayID);
		RendererSettingsDebug.points.setSizeData(0);
	}
	void renderLines(render::Camera* camera, mathem::Transform* cameraTransform)
	{
		unsigned int shaderIndex = 2;
		if (!standardShaders[shaderIndex]->use())
			return;

		unsigned int amountOfLines = RendererSettingsDebug.lines.getSizeData();
		if (amountOfLines <= 0)
			return;

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 positionTransform = glm::mat4(1.0f);
		positionTransform = proj * view;
		standardShaders[shaderIndex]->setMat4("positionTransform", positionTransform);

		GLint posAttrib = glGetAttribLocation(standardShaders[shaderIndex]->textureID, "position");
		GLint colorAttrib = glGetAttribLocation(standardShaders[shaderIndex]->textureID, "color");

		unsigned int linesVBO;
		unsigned int linesArrayID;
		glGenVertexArrays(1, &linesArrayID);
		glGenBuffers(1, &linesVBO);
		glBindVertexArray(linesArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * amountOfLines,
			RendererSettingsDebug.lines.data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		glDrawArrays(GL_LINES, 0, amountOfLines * 14);

		glDeleteBuffers(1, &linesArrayID);
		RendererSettingsDebug.lines.setSizeData(0);
	}
	void renderTriangles(render::Camera* camera, mathem::Transform* cameraTransform)
	{
		unsigned int shaderIndex = 2;
		if (!standardShaders[shaderIndex]->use())
			return;

		unsigned int amountOfTriangles = RendererSettingsDebug.triangles.getSizeData();
		if (amountOfTriangles <= 0)
			return;

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::rotate(view, -glm::radians(cameraTransform->angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::rotate(view, -glm::radians(cameraTransform->angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		view = glm::translate(view, glm::vec3(-cameraTransform->x, -cameraTransform->y, cameraTransform->z));

		glm::mat4 proj = glm::mat4(1.0f);
		proj = glm::perspective(glm::radians(camera->FOV), (float)camera->width / camera->height, camera->nearCullPlane, camera->farCullPlane);

		glm::mat4 positionTransform = glm::mat4(1.0f);
		positionTransform = proj * view;
		standardShaders[shaderIndex]->setMat4("positionTransform", positionTransform);

		GLint posAttrib = glGetAttribLocation(standardShaders[shaderIndex]->textureID, "position");
		GLint colorAttrib = glGetAttribLocation(standardShaders[shaderIndex]->textureID, "color");

		unsigned int trianglesVBO;
		unsigned int trianglesArrayID;
		glGenVertexArrays(1, &trianglesArrayID);
		glGenBuffers(1, &trianglesVBO);
		glBindVertexArray(trianglesArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, trianglesVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * amountOfTriangles,
			RendererSettingsDebug.triangles.data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(colorAttrib);
		glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, amountOfTriangles * 21);
		if (RendererSettingsDebug.backfaceCulling)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}

		glDeleteBuffers(1, &trianglesArrayID);
		RendererSettingsDebug.triangles.setSizeData(0);
	}
}