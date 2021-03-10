
#ifndef SPOTLIGHT_CALLBACKS_H
#define SPOTLIGHT_CALLBACKS_H

#include "engine/MiteVox/src/MiteVox.h"
#include "engine/ECSManager/src/EntityComponentSystem.h"

inline void SpotLight_onUpdateAll(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data)
{
	for (entityID entityIndex = 0;
		entityIndex < _ecs->componentManagers[_managerIndex]->amountOfInstances;
		entityIndex++)
	{
		entityID ID =
			_ecs->componentManagers[_managerIndex]->componentToID[entityIndex];

		unsigned int shaderIndex = 0;
		if (!render::shaders[shaderIndex]->use())
			return;

		mathem::Transform* transform =
			(mathem::Transform*)_ecs->getComponent(ID, TRANSFORM_COMPONENT);

		render::SpotLight* spotLight =
			(render::SpotLight*)_ecs->getComponent(ID, SPOTLIGHT_COMPONENT);
		render::shaders[shaderIndex]->setInt("amountOfSpotLights", entityIndex + 1);

		std::string spotLights = "spotLights[";
		std::string indexStr = std::to_string(entityIndex);
		std::string pos = "].pos";
		std::string direction = "].direction";
		std::string angle = "].angle";
		std::string color = "].color";
		std::string constant = "].constant";
		std::string linear = "].linear";
		std::string quadratic = "].quadratic";

		std::string posResult = spotLights + indexStr + pos;
		std::string directionResult = spotLights + indexStr + direction;
		std::string angleResult = spotLights + indexStr + angle;
		std::string colorResult = spotLights + indexStr + color;
		std::string constantResult = spotLights + indexStr + constant;
		std::string linearResult = spotLights + indexStr + linear;
		std::string quadraticResult = spotLights + indexStr + quadratic;

		glm::vec3 tmpPos;

		tmpPos.x = spotLight->position.x + transform->x;
		tmpPos.y = spotLight->position.y + transform->y;
		tmpPos.z = spotLight->position.z + transform->z;
		render::shaders[shaderIndex]->setVec3(posResult.c_str(), tmpPos.x, tmpPos.y, tmpPos.z);
		render::shaders[shaderIndex]->setVec3(directionResult.c_str(),
			spotLight->direction.i, spotLight->direction.j, spotLight->direction.k);
		render::shaders[shaderIndex]->setFloat(angleResult.c_str(), spotLight->angle);

		render::shaders[shaderIndex]->setVec3(colorResult.c_str(), spotLight->color.r, spotLight->color.g, spotLight->color.b);

		render::shaders[shaderIndex]->setFloat(constantResult.c_str(), spotLight->constant);
		render::shaders[shaderIndex]->setFloat(linearResult.c_str(), spotLight->linear);
		render::shaders[shaderIndex]->setFloat(quadraticResult.c_str(), spotLight->quadratic);
	}
}

inline void SpotLight_onDelete(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index)
{
	unsigned int shaderIndex = 0;
	if (!render::shaders[shaderIndex]->use())
		return;
	render::shaders[shaderIndex]->setInt("amountOfSpotLights", 0);
}

#endif