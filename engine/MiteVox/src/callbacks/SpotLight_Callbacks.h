
#ifndef SPOTLIGHT_CALLBACKS_H
#define SPOTLIGHT_CALLBACKS_H

void SpotLight_onUpdate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	unsigned int shaderIndex = 0;
	if (!render::standardShaders[shaderIndex]->use())
		return;

	mathem::Transform* transform =
		_ecs->getComponent<mathem::Transform>(entityID, TRANSFORM_COMPONENT);

	render::SpotLight* spotLight =
		_ecs->getComponent<render::SpotLight>(entityID, SPOTLIGHT_COMPONENT);
	render::standardShaders[shaderIndex]->setInt("amountOfSpotLights", index + 1);

	std::string spotLights = "spotLights[";
	std::string indexStr = std::to_string(index);
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
	render::standardShaders[shaderIndex]->setVec3(posResult.c_str(), tmpPos.x, tmpPos.y, tmpPos.z);
	render::standardShaders[shaderIndex]->setVec3(directionResult.c_str(),
		spotLight->direction.i, spotLight->direction.j, spotLight->direction.k);
	render::standardShaders[shaderIndex]->setFloat(angleResult.c_str(), spotLight->angle);

	render::standardShaders[shaderIndex]->setVec3(colorResult.c_str(), spotLight->color.r, spotLight->color.g, spotLight->color.b);

	render::standardShaders[shaderIndex]->setFloat(constantResult.c_str(), spotLight->constant);
	render::standardShaders[shaderIndex]->setFloat(linearResult.c_str(), spotLight->linear);
	render::standardShaders[shaderIndex]->setFloat(quadraticResult.c_str(), spotLight->quadratic);
}

void SpotLight_onDelete(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	unsigned int shaderIndex = 0;
	if (!render::standardShaders[shaderIndex]->use())
		return;
	render::standardShaders[shaderIndex]->setInt("amountOfSpotLights", 0);
}

#endif