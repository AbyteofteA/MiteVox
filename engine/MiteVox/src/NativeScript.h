
#ifndef NATIVESCRIPT_H
#define NATIVESCRIPT_H

struct NativeScript_ECS
{
	void (*onCreate)(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data) = doNothing;
	void (*onUpdate)(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data) = doNothing;
	void (*onDestroy)(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data) = doNothing;
	void (*onDebug)(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data) = doNothing;
};

#endif