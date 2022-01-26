
#ifndef DIRECTEDLIGHT_CALLBACKS_H
#define DIRECTEDLIGHT_CALLBACKS_H

#include "engine/MiteVox/src/MiteVox.h"
#include "engine/ECSManager/src/EntityComponentSystem.h"

inline void DirectedLight_onUpdateAll(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data)
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

		render::DirectedLight* directedLight =
			(render::DirectedLight*)_ecs->getComponent(ID, DIRECTEDLIGHT_COMPONENT);
		render::shaders[shaderIndex]->setInt("amountOfDirectionalLights", entityIndex + 1);

		std::string directedLights = "directionalLights[";
		std::string indexStr = std::to_string(entityIndex);
		std::string direction = "].direction";
		std::string color = "].color";
		std::string intensity = "].intensity";
		std::string range = "].range";

		std::string directionResult = directedLights + indexStr + direction;
		std::string colorResult = directedLights + indexStr + color;
		std::string intensityResult = directedLights + indexStr + intensity;
		std::string rangeResult = directedLights + indexStr + range;

		render::shaders[shaderIndex]->setVec3(directionResult.c_str(),
			directedLight->direction.i, directedLight->direction.j, directedLight->direction.k);
		render::shaders[shaderIndex]->setVec3(colorResult.c_str(),
			directedLight->color.r, directedLight->color.g, directedLight->color.b);

		render::shaders[shaderIndex]->setFloat(intensityResult.c_str(), directedLight->intensity);
		render::shaders[shaderIndex]->setFloat(rangeResult.c_str(), directedLight->range);
	}
}

inline void DirectedLight_onDelete(ecs::EntityComponentSystem<entityID>* _ecs, unsigned char _managerIndex, entityID ID, void* data, entityID index)
{
	unsigned int shaderIndex = 0;
	if (!render::shaders[shaderIndex]->use())
		return;

	render::shaders[shaderIndex]->setInt("amountOfDirectionalLights", 0);
}

#endif