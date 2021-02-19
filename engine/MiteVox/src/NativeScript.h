
#ifndef NATIVESCRIPT_H
#define NATIVESCRIPT_H

struct NativeScript_ECS
{
	void (*onCreate)(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data) = nullptr;
	void (*onUpdate)(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data) = nullptr;
	void (*onDestroy)(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data) = nullptr;
	void (*onDebug)(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data) = nullptr;
};

#endif