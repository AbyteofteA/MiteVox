
#ifndef POINTLIGHT_CALLBACKS_H
#define POINTLIGHT_CALLBACKS_H

void PointLight_onUpdateAll(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data)
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

		mathem::Transform* transform =
			(mathem::Transform*)_ecs->getComponent(entityID, TRANSFORM_COMPONENT);

		render::PointLight* pointLight =
			(render::PointLight*)_ecs->getComponent(entityID, POINTLIGHT_COMPONENT);
		render::shaders[shaderIndex]->setInt("amountOfPointLights", entityIndex + 1);

		std::string pointLights = "pointLights[";
		std::string indexStr = std::to_string(entityIndex);
		std::string pos = "].pos";
		std::string color = "].color";
		std::string constant = "].constant";
		std::string linear = "].linear";
		std::string quadratic = "].quadratic";

		std::string posResult = pointLights + indexStr + pos;
		std::string colorResult = pointLights + indexStr + color;
		std::string constantResult = pointLights + indexStr + constant;
		std::string linearResult = pointLights + indexStr + linear;
		std::string quadraticResult = pointLights + indexStr + quadratic;

		glm::vec3 tmpPos;

		tmpPos.x = transform->x + pointLight->position.x;
		tmpPos.y = transform->y + pointLight->position.y;
		tmpPos.z = transform->z + pointLight->position.z;
		render::shaders[shaderIndex]->setVec3(posResult.c_str(), tmpPos.x, tmpPos.y, tmpPos.z);
		render::shaders[shaderIndex]->setVec3(colorResult.c_str(), pointLight->color.r, pointLight->color.g, pointLight->color.b);

		render::shaders[shaderIndex]->setFloat(constantResult.c_str(), pointLight->constant);
		render::shaders[shaderIndex]->setFloat(linearResult.c_str(), pointLight->linear);
		render::shaders[shaderIndex]->setFloat(quadraticResult.c_str(), pointLight->quadratic);
	}
}

void PointLight_onDelete(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	unsigned int shaderIndex = 0;
	if (!render::shaders[shaderIndex]->use())
		return;
	render::shaders[shaderIndex]->setInt("amountOfPointLights", 0);
}

#endif