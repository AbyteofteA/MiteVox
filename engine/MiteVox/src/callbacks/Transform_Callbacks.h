
#ifndef TRANSFORM_CALLBACKS_H
#define TRANSFORM_CALLBACKS_H

void Transform_onCreate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	if (!data)
	{
		*(mathem::Transform*)_ecs->componentManagers[TRANSFORM_COMPONENT].getComponentLocation(entityID)
			= mathem::Transform();
	}
}

#endif