#include "Camera.h"

#include "engine/Math/src/Utilities/Convertations.h"

namespace render
{
	Camera::Camera()
	{

	}

	Camera::Camera(float _FOV, int _width, int _height, float _nearCullPlane, float _farCullPlane)
	{
		FOV = _FOV;
		aspectRatio = (float)_width / (float)_height;
		nearCullPlane = _nearCullPlane;
		farCullPlane = _farCullPlane;
	}

	void Camera::updateMatrices(mathem::GeometryTransform* cameraTransform)
	{
		viewMatrix = makeViewMatrix(cameraTransform);
		viewOrientationMatrix = makeViewOrientationMatrix(cameraTransform);
		projectionMatrix = glm::mat4(1.0f);
		projectionMatrix = glm::perspective(glm::radians(FOV), aspectRatio, nearCullPlane, farCullPlane);
	}

	glm::mat4 Camera::makeViewOrientationMatrix(mathem::GeometryTransform* cameraTransform)
	{
		cameraTransform->rotation = cameraTransform->rotation.lookRotation(upDirection);

		float cameraRotationX, cameraRotationY, cameraRotationZ = 0.0f;
		cameraTransform->rotation.toEulersRadians(&cameraRotationX, &cameraRotationY, &cameraRotationZ);

		glm::mat4 viewMatrix = glm::mat4(1.0f);
		viewMatrix = glm::rotate(viewMatrix, -cameraRotationX, glm::vec3(1.0f, 0.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, -cameraRotationY, glm::vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, -cameraRotationZ, glm::vec3(0.0f, 0.0f, 1.0f));

		return viewMatrix;
	}

	glm::mat4 Camera::makeViewMatrix(mathem::GeometryTransform* cameraTransform)
	{
		glm::mat4 viewMatrix = makeViewOrientationMatrix(cameraTransform);
		viewMatrix = glm::translate(viewMatrix, glm::vec3(-cameraTransform->translation.x(), -cameraTransform->translation.y(), -cameraTransform->translation.z()));
		
		return viewMatrix;
	}

	glm::mat4 Camera::getViewMatrix()
	{
		return viewMatrix;
	}

	glm::mat4 Camera::getViewOrientationMatrix()
	{
		return viewOrientationMatrix;
	}

	glm::mat4 Camera::getProjectionMatrix()
	{
		return projectionMatrix;
	}
}