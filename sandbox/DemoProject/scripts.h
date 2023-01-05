
#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Math/src/Math.h"

void processInput_Script(mitevox::Scene* scene)
{
	float cameraSensitivity = 0.1f;
	float speed = 0.5f;

	InputHandler* inputHandler = scene->settings->getInputHandler();
	mathem::GeometryTransform* cameraTransform = scene->activeCameraNode->getTransform();
	render::Camera* camera = scene->activeCameraNode->camera;

	int windowWidth, windowHeight;
	inputHandler->getWindowSize(&windowWidth, &windowHeight);
	cameraTransform->rotation.rotateByEulers(
		((float)inputHandler->mouseDeltaY) * cameraSensitivity,
		((float)inputHandler->mouseDeltaX) * cameraSensitivity,
		0.0f);

	float cameraRotationX, cameraRotationY, cameraRotationZ = 0.0f;
	cameraTransform->rotation.toEulers(&cameraRotationX, &cameraRotationY, &cameraRotationZ);
	camera->FOV -= inputHandler->mouseDeltaScroll;
	inputHandler->mouseDeltaScroll = 0.0f; // TODO:

	float movementStep = speed * (float)inputHandler->dt;

	mathem::Vector3D movementVector = { 0.0f, 0.0f, 0.0f };
	if (inputHandler->keyW)
	{
		movementVector.z() = -1.0f;
	}
	else if (inputHandler->keyS)
	{
		movementVector.z() = 1.0f;
	}
	if (inputHandler->keyD)
	{
		movementVector.x() = 1.0f;
	}
	else if (inputHandler->keyA)
	{
		movementVector.x() = -1.0f;
	}
	if (inputHandler->keySpace)
	{
		movementVector.y() = 1.0f;
	}
	else if (inputHandler->keyLShift)
	{
		movementVector.y() -= 1.0f;
	}
	movementVector.normalize();
	movementVector *= movementStep;
	movementVector = cameraTransform->rotation.rotate(movementVector);
	cameraTransform->translation.x() += movementVector.x();
	cameraTransform->translation.y() += movementVector.y();
	cameraTransform->translation.z() += movementVector.z();

	/*mitevox::Node* zeroNode = scene->nodes.getElement(0);
	zeroNode->transform.rotation.rotateByEulers(0.0f, 90.0f * (float)inputHandler->dt, 0.0f);*/
}

#endif