
#ifndef NATIVESCRIPT_CALLBACKS_H
#define NATIVESCRIPT_CALLBACKS_H

#include "engine/MiteVox/src/MiteVox.h"
#include "engine/ECSManager/src/EntityComponentSystem.h"

inline void NativeScript_onCreate(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index)
{
	NativeScript_ECS* script =
		(NativeScript_ECS*)_ecs->getComponent(ID, NATIVE_SCRIPT_COMPONENT);
	if (data)
	{
		*script = *(NativeScript_ECS*)data;
	}
	else
	{
		*script = { nullptr };
	}
	if (script->onCreate)
	{
		script->onCreate(_ecs, _managerIndex, ID, data);
	}
}

inline void NativeScript_onUpdateAll(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data)
{
	for (entityID entityIndex = 0;
		entityIndex < _ecs->componentManagers[_managerIndex]->amountOfInstances;
		entityIndex++)
	{
		entityID ID =
			_ecs->componentManagers[_managerIndex]->componentToID[entityIndex];

		NativeScript_ECS* script =
			(NativeScript_ECS*)_ecs->getComponent(ID, NATIVE_SCRIPT_COMPONENT);
		if (script->onUpdate)
		{
			script->onUpdate(_ecs, _managerIndex, ID, data);
		}
	}
}

inline void NativeScript_onDestroy(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index)
{
	NativeScript_ECS* script =
		(NativeScript_ECS*)_ecs->getComponent(ID, NATIVE_SCRIPT_COMPONENT);
	if (script->onDestroy)
	{
		script->onDestroy(_ecs, _managerIndex, ID, data);
	}
}

#endif