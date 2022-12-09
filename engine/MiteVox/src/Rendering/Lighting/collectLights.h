#ifndef COLLECTLIGHTS_H
#define COLLECTLIGHTS_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/RendererAPI/Light.h"
#include "engine/MiteVox/src/Playground/Node.h"

namespace mitevox
{
	void collectPointLights(
		safety::SafeArray<Node*>* nodes,
		safety::SafeArray<render::PointLight>* resultPointLightsArray,
		safety::SafeArray<render::DirectionalLight>* resultDirectionalLightsArray,
		safety::SafeArray<render::SpotLight>* resultSpotLightsArray);
}

#endif