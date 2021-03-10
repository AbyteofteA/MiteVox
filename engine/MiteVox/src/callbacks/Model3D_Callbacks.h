
#ifndef MODEL3D_CALLBACKS_H
#define MODEL3D_CALLBACKS_H

#include "engine/MiteVox/src/MiteVox.h"
#include "engine/ECSManager/src/EntityComponentSystem.h"

inline void Model3D_onCreate(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index)
{
	render::uploadModel3D((render::Model3D*)_ecs->getComponent(ID, MODEL3D_COMPONENT));
}

inline void Model3D_onUpdateAll(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data)
{
	for (entityID entityIndex = 0;
		entityIndex < _ecs->componentManagers[_managerIndex]->amountOfInstances;
		entityIndex++)
	{
		entityID ID =
			_ecs->componentManagers[_managerIndex]->componentToID[entityIndex];

		long* activeCamera = (long*)data;

		render::Camera* camera =
			(render::Camera*)_ecs->getComponent(*activeCamera, CAMERA_COMPONENT);

		mathem::Transform* globalTransform =
			(mathem::Transform*)_ecs->getComponent(ID, TRANSFORM_COMPONENT);

		mathem::Transform* cameraTransform =
			(mathem::Transform*)_ecs->getComponent(*activeCamera, TRANSFORM_COMPONENT);

		render::renderModel3D(
			(render::Model3D*)_ecs->getComponent(ID, MODEL3D_COMPONENT),
			globalTransform, camera, cameraTransform);
	}
}

inline void Model3D_onDestroy(ecs::EntityComponentSystem<entityID>* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index)
{
	//render::removeModel3D(
	//	_ecs->getComponent<render::Model3D>(entityID, MODEL3D_COMPONENT));
}

#endif