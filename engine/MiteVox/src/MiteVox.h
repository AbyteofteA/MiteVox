
#ifndef MITEVOX_HEADERS_H
#define MITEVOX_HEADERS_H

//#include "AIModels/src/NN_headers.h"
#include "FileIO/src/FileIO_Headers.h"
#include "Math/src/Math_Headers.h"
#include "Renderer/src/RendererAPI/Renderer_API.h"
#include "Physics/src/Physics_Headers.h"
#include "UIEventHandler/src/UIEventHandler_Headers.h"
#include "ECSManager/src/ECS.h"

// The most important function.
void doNothing() {}
// The most important function for NativeScript_ECS.
void doNothing(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data) {}
// The most important function for ECS callbacks.
void doNothing(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index) {}

#include "NativeScript.h"
#include "MiteVox_Scene.h"
#include "MiteVox_Engine.h"

#endif