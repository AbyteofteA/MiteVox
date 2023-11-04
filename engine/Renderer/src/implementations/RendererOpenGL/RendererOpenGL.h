#ifndef RENDEREROPENGL_H
#define RENDEREROPENGL_H

#ifndef MAX_SHADERS
#define MAX_SHADERS 128
#endif

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/implementations/RendererOpenGL/ShaderOpenGL.h"
#include "engine/Renderer/src/implementations/RendererOpenGL/SpotLightShadowMapOpenGL.h"

namespace render
{
	extern ShaderOpenGL* shaders[MAX_SHADERS]; // TODO: replace with SafeArray
	extern safety::SafeArray<SpotLightShadowMapOpenGL> spotLightShadowMaps;
}

#endif