#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "engine/MiteVox/src/MiteVox.h"
#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Math/src/Math.h"

mathem::Vector3D getMovementDirection(int forwardKey, int backwardKey, int rightKey, int leftKey, int upKey, int downKey)
{
	InputHandler* inputHandler = InputHandler::getInstance();

	mathem::Vector3D movementVector = { 0.0f, 0.0f, 0.0f };
	if (inputHandler->isKeyPressed(forwardKey))
	{
		movementVector.z() = -1.0f;
	}
	else if (inputHandler->isKeyPressed(backwardKey))
	{
		movementVector.z() = 1.0f;
	}
	if (inputHandler->isKeyPressed(rightKey))
	{
		movementVector.x() = 1.0f;
	}
	else if (inputHandler->isKeyPressed(leftKey))
	{
		movementVector.x() = -1.0f;
	}
	if (inputHandler->isKeyPressed(upKey))
	{
		movementVector.y() = 1.0f;
	}
	else if (inputHandler->isKeyPressed(downKey))
	{
		movementVector.y() -= 1.0f;
	}
	movementVector.normalize();
	return movementVector;
}

void processInput_Script(mitevox::Scene* scene)
{
	float cameraSensitivity = 0.1f;
	float speed = 5.5f;

	InputHandler* inputHandler = InputHandler::getInstance();
	mitevox::Entity* activeCameraEntity = mitevox::MiteVoxAPI::getActiveCameraEntity();
	activeCameraEntity->transform.addOrientation(
		{
			((float)inputHandler->mouseDeltaY) * cameraSensitivity,
			((float)inputHandler->mouseDeltaX) * cameraSensitivity,
			0.0f
		}
	);

	// "Up" vector is opposite gravity direction
	mathem::Vector3D upDirection = -mitevox::MiteVoxAPI::getGravity(activeCameraEntity);
	upDirection.normalize();
	activeCameraEntity->transform.setOrientation(
		activeCameraEntity->transform.getOrientation().lookRotation(upDirection));

	//float cameraRotationX, cameraRotationY, cameraRotationZ = 0.0f;
	//cameraEntityTransform->rotation.toEulers(&cameraRotationX, &cameraRotationY, &cameraRotationZ);
	//camera->FOV -= inputHandler->mouseDeltaScroll;
	//inputHandler->mouseDeltaScroll = 0.0f; // TODO:

	float movementStep = speed * (float)inputHandler->dt;

	mathem::Vector3D movementVector = movementVector = getMovementDirection(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT);
	movementVector *= movementStep;
	movementVector = activeCameraEntity->transform.getOrientation().rotate(movementVector);
	activeCameraEntity->transform.addPosition(movementVector);

	if (inputHandler->isKeyPressed(GLFW_KEY_1))
	{
		auto entitiesToSimulate = mitevox::MiteVoxAPI::collectEntitiesToSimulate();
		size_t entitiesCount = entitiesToSimulate->getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			mitevox::Entity* entity = entitiesToSimulate->getElement(i);
			if (entity->renderableNode)
			{
				entity->renderableNode->setIlluminationModelRecursively(mitevox::IlluminationModel::UNLIT);
			}
		}
	}

	if (inputHandler->isKeyPressed(GLFW_KEY_2))
	{
		auto entitiesToSimulate = mitevox::MiteVoxAPI::collectEntitiesToSimulate();
		size_t entitiesCount = entitiesToSimulate->getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			mitevox::Entity* entity = entitiesToSimulate->getElement(i);
			if (entity->renderableNode)
			{
				entity->renderableNode->setIlluminationModelRecursively(mitevox::IlluminationModel::PHONG);
			}
		}
	}

	if (inputHandler->isKeyPressed(GLFW_KEY_3))
	{
		auto entitiesToSimulate = mitevox::MiteVoxAPI::collectEntitiesToSimulate();
		size_t entitiesCount = entitiesToSimulate->getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			mitevox::Entity* entity = entitiesToSimulate->getElement(i);
			if (entity->renderableNode)
			{
				entity->renderableNode->setIlluminationModelRecursively(mitevox::IlluminationModel::PBR);
			}
		}
	}

	if (inputHandler->isKeyPressed(GLFW_KEY_L))
	{
		mitevox::MiteVoxAPI::getSettings()->debug = true;
	}
	if (inputHandler->isKeyPressed(GLFW_KEY_P))
	{
		mitevox::MiteVoxAPI::getSettings()->debug = false;
	}

	static float timeSinceLastDrop = 0.0f;
	timeSinceLastDrop += inputHandler->dt;
	if (timeSinceLastDrop > 0.333f)
	{
		if (inputHandler->isKeyPressed(GLFW_KEY_Q))
		{
			timeSinceLastDrop = 0.0f;
			mitevox::Entity* e = mitevox::MiteVoxAPI::createBox(
				"Dropped Cube",
				mitevox::MiteVoxAPI::getRandom(0.15f, 1.0f),
				mitevox::MiteVoxAPI::getRandom(0.15f, 0.5f),
				mitevox::MiteVoxAPI::getRandom(0.15f, 1.0f),
				activeCameraEntity->transform.getPosition(),
				{
					mitevox::MiteVoxAPI::getRandom(0.05f, 1.0f),
					mitevox::MiteVoxAPI::getRandom(0.05f, 1.0f),
					mitevox::MiteVoxAPI::getRandom(0.05f, 1.0f),
					1.0f
				});
			e->movementProperties.velocity = activeCameraEntity->getViewRay() * 10.0f;
		}
	}
	
	//if (inputHandler->isKeyPressed(GLFW_KEY_F))
	//{
	//	auto entity = mitevox::MiteVoxAPI::getActiveCameraEntity();
	//	entity.s
	//}
}

#endif