#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "engine/MiteVox/src/MiteVox.h"
#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Math/src/Math.h"

void processInput_Script(mitevox::Scene* scene)
{
	float cameraSensitivity = 0.1f;
	float speed = 5.5f;

	InputHandler* inputHandler = InputHandler::getInstance();
	mitevox::Entity* activeCameraEntity = mitevox::MiteVoxAPI::getActiveCameraEntity();
	mathem::GeometryTransform* cameraEntityTransform = &activeCameraEntity->transform;

	cameraEntityTransform->rotation.rotateByEulers(
		((float)inputHandler->mouseDeltaY) * cameraSensitivity,
		((float)inputHandler->mouseDeltaX) * cameraSensitivity,
		0.0f);

	// "Up" vector is opposite gravity direction
	mathem::Vector3D upDirection = -mitevox::MiteVoxAPI::getGravity(activeCameraEntity);
	upDirection.normalize();
	cameraEntityTransform->rotation = cameraEntityTransform->rotation.lookRotation(upDirection);

	//float cameraRotationX, cameraRotationY, cameraRotationZ = 0.0f;
	//cameraEntityTransform->rotation.toEulers(&cameraRotationX, &cameraRotationY, &cameraRotationZ);
	//camera->FOV -= inputHandler->mouseDeltaScroll;
	//inputHandler->mouseDeltaScroll = 0.0f; // TODO:

	float movementStep = speed * (float)inputHandler->dt;

	mathem::Vector3D movementVector = { 0.0f, 0.0f, 0.0f };
	if (inputHandler->isKeyPressed(GLFW_KEY_W))
	{
		movementVector.z() = -1.0f;
	}
	else if (inputHandler->isKeyPressed(GLFW_KEY_S))
	{
		movementVector.z() = 1.0f;
	}
	if (inputHandler->isKeyPressed(GLFW_KEY_D))
	{
		movementVector.x() = 1.0f;
	}
	else if (inputHandler->isKeyPressed(GLFW_KEY_A))
	{
		movementVector.x() = -1.0f;
	}
	if (inputHandler->isKeyPressed(GLFW_KEY_SPACE))
	{
		movementVector.y() = 1.0f;
	}
	else if (inputHandler->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		movementVector.y() -= 1.0f;
	}
	movementVector.normalize();
	movementVector *= movementStep;
	movementVector = cameraEntityTransform->rotation.rotate(movementVector);
	cameraEntityTransform->translation.x() += movementVector.x();
	cameraEntityTransform->translation.y() += movementVector.y();
	cameraEntityTransform->translation.z() += movementVector.z();

	if (inputHandler->isKeyPressed(GLFW_KEY_Q))
	{
		mitevox::Entity* e = mitevox::MiteVoxAPI::createCube(
			"Dropped Cube",
			cameraEntityTransform->translation,
			{ 
				1.0f * (float)((*(uint32_t*)&inputHandler->dt) % 2),
				1.0f * (float)((*(uint32_t*)&inputHandler->dt) % 3),
				1.0f - 1.0f * (float)((*(uint32_t*)&inputHandler->dt) % 2), 1.0f});
		e->movementProperties.velocity = activeCameraEntity->getViewRay() * 10.0f;
	}
}

#endif