
#ifndef DIRECTEDLIGHT_CALLBACKS_H
#define DIRECTEDLIGHT_CALLBACKS_H

void DirectedLight_onUpdateAll(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data)
{
	for (COMPONENT_TYPE entityIndex = 0;
		entityIndex < _ecs->componentManagers[_managerIndex].amountOfInstances;
		entityIndex++)
	{
		COMPONENT_TYPE entityID =
			_ecs->componentManagers[_managerIndex].componentToID[entityIndex];

		unsigned int shaderIndex = 0;
		if (!render::shaders[shaderIndex]->use())
			return;

		render::DirectedLight* directedLight =
			(render::DirectedLight*)_ecs->getComponent(entityID, DIRECTEDLIGHT_COMPONENT);
		render::shaders[shaderIndex]->setInt("amountOfDirectionalLights", entityIndex + 1);

		std::string directedLights = "directionalLights[";
		std::string indexStr = std::to_string(entityIndex);
		std::string direction = "].direction";
		std::string color = "].color";

		std::string directionResult = directedLights + indexStr + direction;
		std::string colorResult = directedLights + indexStr + color;

		render::shaders[shaderIndex]->setVec3(directionResult.c_str(),
			directedLight->direction.i, directedLight->direction.j, directedLight->direction.k);
		render::shaders[shaderIndex]->setVec3(colorResult.c_str(),
			directedLight->color.r, directedLight->color.g, directedLight->color.b);
	}
}

void DirectedLight_onDelete(ecs::ECS* _ecs, unsigned char _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	unsigned int shaderIndex = 0;
	if (!render::shaders[shaderIndex]->use())
		return;

	render::shaders[shaderIndex]->setInt("amountOfDirectionalLights", 0);
}

#endif