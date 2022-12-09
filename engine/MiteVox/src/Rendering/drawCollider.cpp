#include "drawCollider.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"

namespace mitevox
{
	void drawCollider(render::RendererSettings* renderer, mathem::GeometryPrimitiveBase* geometryPrimitiveBox, mathem::GeometryTransform transform)
	{
		render::ColorRGBAf colliderColor = { 1.0, 1.0, 1.0, 1.0 };

		render::Point points[8];
		size_t colliderVertecesCount = geometryPrimitiveBox->getVertecesCount();
		for (size_t i = 0; i < colliderVertecesCount; ++i)
		{
			mathem::Vector3D position = geometryPrimitiveBox->getVertexPosition(i);
			transform.applyTo(position);

			points[i].color = colliderColor;
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

	void drawCollider(render::RendererSettings* renderer, mathem::ComplexGeometry* collider, mathem::GeometryTransform* transform)
	{
		size_t colliderPrimitivesCount = collider->primitives.getElementsCount();
		for (size_t i = 0; i < colliderPrimitivesCount; ++i)
		{
			mathem::GeometryPrimitiveBase* colliderPrimitive = collider->primitives.getElement(i);
			mathem::GeometryPrimitiveType colliderPrimitiveType = colliderPrimitive->getType();
			switch (colliderPrimitiveType)
			{
			case mathem::GeometryPrimitiveType::AXIS_ALIGNED_BOX:
				drawCollider(renderer, (mathem::AxisAlignedBoxGeometry*)colliderPrimitive, *transform);
				break;

			case mathem::GeometryPrimitiveType::BOX:
				drawCollider(renderer, (mathem::BoxGeometry*)colliderPrimitive, *transform);
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
}