
#ifndef MODEL3D_CALLBACKS_H
#define MODEL3D_CALLBACKS_H

void Model3D_onCreate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	render::uploadModel3D(
		(render::Model3D*)_ecs->componentManagers[MODEL3D_COMPONENT].getComponentLocation(entityID));
}
void Model3D_onUpdate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	long* activeCamera = (long*)data;

	render::Camera* camera = (render::Camera*)_ecs->componentManagers[CAMERA_COMPONENT].getComponentLocation(*activeCamera);

	mathem::Transform* globalTransform =
		(mathem::Transform*)_ecs->componentManagers[TRANSFORM_COMPONENT].getComponentLocation(entityID);

	mathem::Transform* cameraTransform =
		(mathem::Transform*)_ecs->componentManagers[TRANSFORM_COMPONENT].getComponentLocation(*activeCamera);

	render::renderModel3D(
		(render::Model3D*)_ecs->componentManagers[MODEL3D_COMPONENT].getComponentLocation(entityID), globalTransform, camera, cameraTransform);
}
void Model3D_onDestroy(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	render::removeModel3D(
		(render::Model3D*)_ecs->componentManagers[MODEL3D_COMPONENT].getComponentLocation(entityID));
}

#endif