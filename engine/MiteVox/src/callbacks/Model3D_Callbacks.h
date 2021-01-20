
#ifndef MODEL3D_CALLBACKS_H
#define MODEL3D_CALLBACKS_H

void Model3D_onCreate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	render::uploadModel3D((render::Model3D*)_ecs->getComponent(entityID, MODEL3D_COMPONENT));
}

void Model3D_onUpdateAll(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data)
{
	for (COMPONENT_TYPE entityIndex = 0;
		entityIndex < _ecs->componentManagers[_managerIndex].amountOfInstances;
		entityIndex++)
	{
		COMPONENT_TYPE entityID =
			_ecs->componentManagers[_managerIndex].componentToID[entityIndex];

		long* activeCamera = (long*)data;

		render::Camera* camera =
			(render::Camera*)_ecs->getComponent(*activeCamera, CAMERA_COMPONENT);

		mathem::Transform* globalTransform =
			(mathem::Transform*)_ecs->getComponent(entityID, TRANSFORM_COMPONENT);

		mathem::Transform* cameraTransform =
			(mathem::Transform*)_ecs->getComponent(*activeCamera, TRANSFORM_COMPONENT);

		render::renderModel3D(
			(render::Model3D*)_ecs->getComponent(entityID, MODEL3D_COMPONENT),
			globalTransform, camera, cameraTransform);
	}
}

void Model3D_onDestroy(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	//render::removeModel3D(
	//	_ecs->getComponent<render::Model3D>(entityID, MODEL3D_COMPONENT));
}

#endif