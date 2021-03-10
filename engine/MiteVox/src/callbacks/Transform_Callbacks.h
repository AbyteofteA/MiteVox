
#ifndef TRANSFORM_CALLBACKS_H
#define TRANSFORM_CALLBACKS_H

#include "engine/MiteVox/src/MiteVox.h"
#include "engine/ECSManager/src/EntityComponentSystem.h"

inline void Transform_onCreate(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index)
{
	if (!data)
	{
		*(mathem::Transform*)_ecs->getComponent(ID, TRANSFORM_COMPONENT) = mathem::Transform();
	}
}

#endif