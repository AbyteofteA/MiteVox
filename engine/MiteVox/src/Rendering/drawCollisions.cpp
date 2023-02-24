#include "drawCollisions.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "drawCollider.h"

namespace mitevox
{
	void drawCollisions(render::RendererSettings* renderer, safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions)
	{
		render::ColorRGBAf redColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		render::ColorRGBAf greenColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		render::ColorRGBAf blueColor = { 0.0f, 0.0f, 1.0f, 1.0f };
		mathem::GeometryTransform zeroTransform;

		size_t collisionsCount = collisions->getElementsCount();
		for (size_t i = 0; i < collisionsCount; ++i)
		{
			mathem::CollisionInfo<Entity*> collision = collisions->getElement(i);
			mathem::ComplexGeometry* complexGeometry1 = collision.object1->getCollider();
			mathem::GeometryTransform* complexGeometryTransform1 = collision.object1->getTransform();
			mathem::ComplexGeometry* complexGeometry2 = collision.object2->getCollider();
			mathem::GeometryTransform* complexGeometryTransform2 = collision.object2->getTransform();
			drawCollider(renderer, complexGeometry1, complexGeometryTransform1, redColor);
			drawCollider(renderer, complexGeometry2, complexGeometryTransform2, blueColor);

			mathem::Vector3D normalOrigin;
			if (collision.properties.normalBelongsToTheFirst)
			{
				normalOrigin = complexGeometryTransform1->translation;
			}
			else
			{
				normalOrigin = complexGeometryTransform2->translation;
			}
			render::Point normalPoint1, normalPoint2;
			normalPoint1.position = normalOrigin;
			normalPoint1.color = greenColor;
			normalPoint2.position = normalOrigin + collision.properties.normal;
			normalPoint2.color = greenColor;
			render::drawLine(renderer, normalPoint1, normalPoint2);

			for (size_t i = 0; i < collision.properties.contactPointsCount; ++i)
			{
				render::Point lightPoint;
				lightPoint.position = collision.properties.contactPoints[i];
				lightPoint.color = greenColor;
				render::drawCross(renderer, lightPoint, 0.125f);
			}
		}
	}
}