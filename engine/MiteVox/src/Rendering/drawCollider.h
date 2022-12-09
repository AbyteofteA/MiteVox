#ifndef DRAWCOLLIDER_H
#define DRAWCOLLIDER_H

#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"

namespace mitevox
{
	void drawCollider(render::RendererSettings* renderer, mathem::GeometryPrimitiveBase* geometryPrimitiveBox, mathem::GeometryTransform transform);
	void drawCollider(render::RendererSettings* renderer, mathem::ComplexGeometry* collider, mathem::GeometryTransform* transform);
}

#endif