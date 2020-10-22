
#ifndef DIRECTEDLIGHT_CALLBACKS_H
#define DIRECTEDLIGHT_CALLBACKS_H

void DirectedLight_onUpdate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	unsigned int shaderIndex = 0;
	if (!render::standardShaders[shaderIndex]->use())
		return;

	render::DirectedLight* directedLight =
		_ecs->getComponent<render::DirectedLight>(entityID, DIRECTEDLIGHT_COMPONENT);
	render::standardShaders[shaderIndex]->setInt("amountOfDirectionalLights", index + 1);

	std::string directedLights = "directionalLights[";
	std::string indexStr = std::to_string(index);
	std::string direction = "].direction";
	std::string color = "].color";

	std::string directionResult = directedLights + indexStr + direction;
	std::string colorResult = directedLights + indexStr + color;

	render::standardShaders[shaderIndex]->setVec3(directionResult.c_str(), 
		directedLight->direction.i, directedLight->direction.j, directedLight->direction.k);
	render::standardShaders[shaderIndex]->setVec3(colorResult.c_str(),
		directedLight->color.r, directedLight->color.g, directedLight->color.b);
}

void DirectedLight_onDelete(ecs::ECS* _ecs, unsigned char _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	unsigned int shaderIndex = 0;
	if (!render::standardShaders[shaderIndex]->use())
		return;

	render::standardShaders[shaderIndex]->setInt("amountOfDirectionalLights", 0);
}

#endif