
#ifndef MODEL3D_CALLBACKS_H
#define MODEL3D_CALLBACKS_H

void Model3D_onCreate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	render::uploadModel3D(_ecs->getComponent<render::Model3D>(entityID, MODEL3D_COMPONENT));
}
void Model3D_onUpdate(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	long* activeCamera = (long*)data;

	render::Camera* camera = 
		_ecs->getComponent<render::Camera>(*activeCamera, CAMERA_COMPONENT);

	mathem::Transform* globalTransform =
		_ecs->getComponent<mathem::Transform>(entityID, TRANSFORM_COMPONENT);

	mathem::Transform* cameraTransform =
		_ecs->getComponent<mathem::Transform>(*activeCamera, TRANSFORM_COMPONENT);

	render::renderModel3D(
		_ecs->getComponent<render::Model3D>(entityID, MODEL3D_COMPONENT), 
		globalTransform, camera, cameraTransform);
}
void Model3D_onDestroy(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index)
{
	//render::removeModel3D(
	//	_ecs->getComponent<render::Model3D>(entityID, MODEL3D_COMPONENT));
}

#endif