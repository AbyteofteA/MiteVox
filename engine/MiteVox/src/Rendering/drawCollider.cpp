#include "drawCollider.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"
#include "drawGeometryPrimitive.h"

namespace mitevox
{
	void drawCollider(
		render::RendererSettings* renderer,
		mathem::ComplexGeometry* collider,
		mathem::GeometryTransform* transform,
		render::ColorRGBAf color)
	{
		size_t colliderPrimitivesCount = collider->primitives.getElementsCount();
		for (size_t i = 0; i < colliderPrimitivesCount; ++i)
		{
			mathem::GeometryPrimitiveBase* colliderPrimitive = collider->primitives.getElement(i);
			drawGeometryPrimitive(renderer, colliderPrimitive, transform, color);
		}
	}
}