#include "drawGeometryPrimitive.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"

namespace mitevox
{
	void drawBoxPrimitive(
		render::RendererSettings* renderer,
		mathem::GeometryPrimitiveBase* geometryPrimitiveBox,
		mathem::GeometryTransform* transform,
		render::ColorRGBAf color)
	{
		render::Point points[8];
		size_t colliderVertecesCount = geometryPrimitiveBox->getVertecesCount();
		for (size_t i = 0; i < colliderVertecesCount; ++i)
		{
			mathem::Vector3D position = geometryPrimitiveBox->getVertexPosition(i);
			transform->applyTo(position);

			points[i].color = color;
			points[i].position = position;
		}
		render::drawLine(renderer, points[0], points[1]);
		render::drawLine(renderer, points[1], points[2]);
		render::drawLine(renderer, points[2], points[3]);
		render::drawLine(renderer, points[3], points[0]);

		render::drawLine(renderer, points[4], points[5]);
		render::drawLine(renderer, points[5], points[7]);
		render::drawLine(renderer, points[7], points[6]);
		render::drawLine(renderer, points[6], points[4]);

		render::drawLine(renderer, points[0], points[7]);
		render::drawLine(renderer, points[1], points[6]);
		render::drawLine(renderer, points[2], points[4]);
		render::drawLine(renderer, points[3], points[5]);
	}

	void drawGeometryPrimitive(
		render::RendererSettings* renderer,
		mathem::GeometryPrimitiveBase* geometryPrimitive,
		mathem::GeometryTransform* transform,
		render::ColorRGBAf color)
	{
		mathem::GeometryPrimitiveType colliderPrimitiveType = geometryPrimitive->getType();
		switch (colliderPrimitiveType)
		{
		case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
			drawBoxPrimitive(renderer, (mathem::AxisAlignedBoxGeometry*)geometryPrimitive, transform, color);
			break;

		case mathem::GeometryPrimitiveType::BOX:
			drawBoxPrimitive(renderer, (mathem::BoxGeometry*)geometryPrimitive, transform, color);
			break;

		case mathem::GeometryPrimitiveType::SPHERE:
			// TODO:
			break;

		case mathem::GeometryPrimitiveType::CAPSULE:
			// TODO:
			break;

		case mathem::GeometryPrimitiveType::TRUNCATED_PYRAMID:
			// TODO:
			break;

		case mathem::GeometryPrimitiveType::RAY:
			// TODO:
			break;

		case mathem::GeometryPrimitiveType::MESH:
			// TODO:
			break;

		default:
			break;
		}
	}
}