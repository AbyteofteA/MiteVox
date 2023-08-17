#ifndef RENDEREROPENGL_H
#define RENDEREROPENGL_H

#ifndef MAX_SHADERS
#define MAX_SHADERS 128
#endif

#include "ShaderOpenGL.h"
namespace render
{
	extern ShaderOpenGL* shaders[MAX_SHADERS]; // TODO: replace with SafeArray
}

#endif