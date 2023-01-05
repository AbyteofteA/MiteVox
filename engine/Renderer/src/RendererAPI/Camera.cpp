#include "Camera.h"

#include "engine/Math/src/Convertations.h"

namespace render
{
	Camera::Camera()
	{

	}

	Camera::Camera(float _FOV, int _width, int _height, float _nearCullPlane, float _farCullPlane)
	{
		FOV = _FOV;
		width = _width;
		height = _height;
		nearCullPlane = _nearCullPlane;
		farCullPlane = _farCullPlane;
	}

	glm::mat4 Camera::getViewProjectionMatrix(mathem::GeometryTransform* cameraTransform)
	{
		cameraTransform->rotation = cameraTransform->rotation.lookRotation(upDirection);

		float cameraRotationX, cameraRotationY, cameraRotationZ = 0.0f;
		cameraTransform->rotation.toEulersRadians(&cameraRotationX, &cameraRotationY, &cameraRotationZ);

		glm::mat4 viewMatrix = glm::mat4(1.0f);
		viewMatrix = glm::rotate(viewMatrix, -cameraRotationX, glm::vec3(1.0f, 0.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, -cameraRotationY, glm::vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, -cameraRotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
		viewMatrix = glm::translate(viewMatrix, glm::vec3(-cameraTransform->translation.x(), -cameraTransform->translation.y(), -cameraTransform->translation.z()));

		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		projectionMatrix = glm::perspective(glm::radians(FOV), (float)width / height, nearCullPlane, farCullPlane);
		glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

		return viewProjectionMatrix;
	}
}