
#ifndef AMBIENTLIGHT_CALLBACKS_H
#define AMBIENTLIGHT_CALLBACKS_H

void AmbientLight_onUpdate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	unsigned int shaderIndex = 0;
	if (!render::standardShaders[shaderIndex]->use())
		return;

	render::AmbientLight* ambientLight =
		(render::AmbientLight*)_ecs->componentManagers[AMBIENTLIGHT_COMPONENT].getComponentLocation(entityID);
	render::standardShaders[shaderIndex]->setVec3("ambientLight.color", ambientLight->color.r, ambientLight->color.g, ambientLight->color.b);
}

#endif