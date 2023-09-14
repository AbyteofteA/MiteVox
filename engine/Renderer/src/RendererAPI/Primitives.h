#ifndef RENDERER_PRIMITIVES_H
#define RENDERER_PRIMITIVES_H

#include "engine/Math/src/Vector.h"
#include "engine/Renderer/src/RendererAPI/Color.h"

namespace render
{
	struct Point
	{
		mathem::Vector3D position;
		//mathem::Vector3D normal;
		//mathem::Vector3D textureCoord;
		render::ColorRGBAf color;
	};
}


#endif