#include "RendererOpenGL.h"

namespace render
{
	ShaderOpenGL* shaders[MAX_SHADERS];
	safety::SafeArray<SpotLightShadowMapOpenGL> spotLightShadowMaps;
}