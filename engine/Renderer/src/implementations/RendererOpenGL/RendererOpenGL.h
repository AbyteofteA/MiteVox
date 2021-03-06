
#ifndef RENDEREROPENGL_H
#define RENDEREROPENGL_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "engine/FileIO/src/FileIO.h"
#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "ShaderOpenGL.h"
namespace render
{
	extern ShaderOpenGL* shaders[MAX_SHADERS];
}
#include "GlobalData.h"

#include "getErrorOpenGL.h"
#include "API_ShaderFunctions.h"
#include "API_GeneralFunctions.h"
#include "API_PrimitiveFunctions.h"
#include "API_ModelFunctions.h"


#endif