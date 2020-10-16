
#ifndef RENDEREROPENGL_H
#define RENDEREROPENGL_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "FileIO/src/FileIO_Headers.h"
#include "Renderer/src/RendererAPI/Renderer_API.h"
#include "ShaderOpenGL.h"
namespace render
{
	extern std::vector<ShaderOpenGL*> standardShaders;

	extern RendererSettings RendererSettingsDebug;
}
#include "GlobalData.h"

#include "getErrorOpenGL.h"
#include "GeneralFunctions.h"
#include "PrimitiveFunctions.h"
#include "ModelFunctions.h"


#endif