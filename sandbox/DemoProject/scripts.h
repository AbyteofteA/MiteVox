#pragma once

#include "engine/MiteVox/src/MiteVox.h"

void rotateLight_Script(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data)
{
	mitevox::Scene* scene = (mitevox::Scene*)data;
	render::DirectedLight* light =
		(render::DirectedLight*)_ecs->getComponent(ID, DIRECTEDLIGHT_COMPONENT);

	light->direction.i = (float)sin(scene->getCurrentTime());
	light->direction.j = -0.5f;
	light->direction.k = (float)cos(scene->getCurrentTime());
	light->direction.normalize();
}

void waveModel_Script(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data)
{
	mitevox::Scene* scene = (mitevox::Scene*)data;
	mathem::Transform* transform =
		(mathem::Transform*)_ecs->getComponent(ID, TRANSFORM_COMPONENT);

	//transform->angleX += 30 * scene->dt;
	//transform->angleY += 60 * scene->dt;
	transform->y = 50 * (float)sin(scene->getCurrentTime() * 4 + transform->x + transform->z);
}

void rotateModel_Script(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data)
{
	mitevox::Scene* scene = (mitevox::Scene*)data;
	mathem::Transform* transform =
		(mathem::Transform*)_ecs->getComponent(ID, TRANSFORM_COMPONENT);

	transform->x = 200 * (float)sin(scene->getCurrentTime() * 2);
	transform->y = 200 * (float)cos(scene->getCurrentTime() * 2);
	transform->z = 0;// 200 * (float)cos(scene->currentTime * 2);
}


void processInput_Script(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data)
{
	float cameraSensitivity = 0.1f;
	float speed = 450;

	mitevox::Scene* scene = (mitevox::Scene*)data;

	InputHandler* inputHandler = scene->inputHandler;

	mathem::Transform* transform =
		(mathem::Transform*)_ecs->getComponent(ID, TRANSFORM_COMPONENT);

	render::Camera* camera = (render::Camera*)_ecs->getComponent(scene->activeCamera, CAMERA_COMPONENT);
	camera->FOV += inputHandler->mouseDeltaScroll;

	float timeForMoves = (float)inputHandler->dt;

	if (inputHandler->keyW)
	{
		transform->x += cos((transform->angleY + 90) * (float)M_PI / 180) * speed * timeForMoves;
		transform->y -= cos((transform->angleX + 90) * (float)M_PI / 180) * speed * timeForMoves;
		transform->z += sin((transform->angleY + 90) * (float)M_PI / 180) * speed * timeForMoves;
	}
	else if (inputHandler->keyS)
	{
		transform->x -= cos((transform->angleY + 90) * (float)M_PI / 180) * speed * timeForMoves;
		transform->y += cos((transform->angleX + 90) * (float)M_PI / 180) * speed * timeForMoves;
		transform->z -= sin((transform->angleY + 90) * (float)M_PI / 180) * speed * timeForMoves;
	}
	if (inputHandler->keyD)
	{
		transform->x += cos(transform->angleY * (float)M_PI / 180) * speed * timeForMoves;
		transform->z += sin(transform->angleY * (float)M_PI / 180) * speed * timeForMoves;
	}
	else if (inputHandler->keyA)
	{
		transform->x -= cos(transform->angleY * (float)M_PI / 180) * speed * timeForMoves;
		transform->z -= sin(transform->angleY * (float)M_PI / 180) * speed * timeForMoves;
	}

	if (inputHandler->keySpace)
	{
		transform->y += speed * timeForMoves;
	}
	else if (inputHandler->keyLShift)
	{
		transform->y -= speed * timeForMoves;
	}

	int windowWidth, windowHeight;
	inputHandler->getWindowSize(&windowWidth, &windowHeight);
	transform->angleY += (-(float)inputHandler->mouseDeltaX + windowWidth / 2) * cameraSensitivity;
	transform->angleX += (-(float)inputHandler->mouseDeltaY + windowHeight / 2) * cameraSensitivity;

	/*render::SpotLight* spotLight = _ecs->getComponent<render::SpotLight>(entityID, SPOTLIGHT_COMPONENT);
	//spotLight->angle += inputHandler->mouseDeltaScroll;
	spotLight->direction = transform->getDirectionVector();*/
}

void rotateCamera_Script(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data)
{
	mitevox::Scene* scene = (mitevox::Scene*)data;
	mathem::Transform* transform =
		(mathem::Transform*)_ecs->getComponent(ID, TRANSFORM_COMPONENT);

	float angle = (float)scene->getCurrentTime() * 50 + 180;

	transform->x = 800 * sin(mathem::toRadians(angle));
	transform->z = 800 * cos(mathem::toRadians(angle));

	transform->angleY = -angle + 180;
}
