#ifndef DRAWCOLLISIONS_H
#define DRAWCOLLISIONS_H

#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/CodeSafety/src/SafeArray.h"

namespace mitevox
{
	void drawCollisions(render::RendererSettings* renderer, safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions);
}

#endif