#ifndef RENDEREROPENGL_H
#define RENDEREROPENGL_H

#ifndef MAX_SHADERS
#define MAX_SHADERS 128
#endif

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/implementations/RendererOpenGL/ShaderOpenGL.h"
#include "engine/Renderer/src/implementations/RendererOpenGL/ShadowMapPack.h"

namespace render
{
	extern ShaderOpenGL* shaders[MAX_SHADERS]; // TODO: replace with SafeArray
	extern ShadowMapPack shadowMapPack;
	extern const float screenQuad[];
	extern const float unitCubePositions[];
}

#endif