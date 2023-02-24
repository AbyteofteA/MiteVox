#ifndef DRAWGEOMETRYPRIMITIVE_H
#define DRAWGEOMETRYPRIMITIVE_H

#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Renderer/src/RendererAPI/Color.h"

namespace mitevox
{
	void drawGeometryPrimitive(
		render::RendererSettings* renderer,
		mathem::GeometryPrimitiveBase* geometryPrimitive,
		mathem::GeometryTransform* transform,
		render::ColorRGBAf color);
}

#endif