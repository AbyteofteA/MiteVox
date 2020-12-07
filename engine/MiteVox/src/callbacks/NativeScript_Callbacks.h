
#ifndef NATIVESCRIPT_CALLBACKS_H
#define NATIVESCRIPT_CALLBACKS_H

void NativeScript_onCreate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	NativeScript_ECS* script =
		(NativeScript_ECS*)_ecs->getComponent(entityID, NATIVE_SCRIPT_COMPONENT);
	if (data)
	{
		*script = *(NativeScript_ECS*)data;
	}
	else
	{
		*script = { doNothing, doNothing, doNothing, doNothing };
	}
	script->onCreate(_ecs, _managerIndex, entityID, data);
}

void NativeScript_onUpdate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	NativeScript_ECS* script =
		(NativeScript_ECS*)_ecs->getComponent(entityID, NATIVE_SCRIPT_COMPONENT);
	script->onUpdate(_ecs, _managerIndex, entityID, data);
}

void NativeScript_onDestroy(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	NativeScript_ECS* script =
		(NativeScript_ECS*)_ecs->getComponent(entityID, NATIVE_SCRIPT_COMPONENT);
	script->onDestroy(_ecs, _managerIndex, entityID, data);
}

#endif