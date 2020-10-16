
#ifndef MITEVOX_HEADERS_H
#define MITEVOX_HEADERS_H

#include "FileIO/src/FileIO_Headers.h"
#include "Math/src/Math_Headers.h"
#include "Renderer/src/RendererAPI/Renderer_API.h"
#include "Physics/src/Physics_Headers.h"
#include "UIEventHandler/src/UIEventHandler_Headers.h"
#include "ECSManager/src/ECS.cpp"

// The most important function.
void doNothing(ecs::ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data) {}

#include "NativeScript.h"
#include "MiteVox_Scene.h"

#endif