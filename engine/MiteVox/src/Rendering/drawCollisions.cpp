#include "drawCollisions.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "drawCollider.h"

#include <cassert>
#undef NDEBUG

namespace mitevox
{
	void drawCollisions(render::RendererSettings* renderer, safety::SafeArray<mathem::CollisionInfo<Entity*>>* collisions)
	{
		mathem::GeometryTransform zeroTransform;

		size_t collisionsCount = collisions->getElementsCount();
		for (size_t i = 0; i < collisionsCount; ++i)
		{
			mathem::CollisionInfo<Entity*> collision = collisions->getElement(i);
			mathem::ComplexGeometry* complexGeometry1 = collision.object1->getCollider();
			mathem::GeometryTransform* complexGeometryTransform1 = collision.object1->getResultTransform();
			mathem::ComplexGeometry* complexGeometry2 = collision.object2->getCollider();
			mathem::GeometryTransform* complexGeometryTransform2 = collision.object2->getResultTransform();
			drawCollider(renderer, complexGeometry1, complexGeometryTransform1, render::ColorRGBAf(render::ColorRGBf::RED()));
			drawCollider(renderer, complexGeometry2, complexGeometryTransform2, render::ColorRGBAf(render::ColorRGBf::BLUE()));

			mathem::Vector3D normalOrigin;
			if (collision.properties.normalBelongsToTheFirst)
			{
				normalOrigin = complexGeometryTransform1->translation;
			}
			else
			{
				assert(collision.properties.normalBelongsToTheFirst == true);
				normalOrigin = complexGeometryTransform2->translation;
			}
			
			render::drawArrow(renderer, normalOrigin, collision.properties.normal, render::ColorRGBAf(render::ColorRGBf::GREEN()));

			for (size_t i = 0; i < collision.properties.contactPointsCount; ++i)
			{
				mathem::Vector3D contactPoints1GlobalCoords = 
					complexGeometryTransform1->applyToCopy(collision.properties.contacts[i].contactPoints1);
				mathem::Vector3D contactPoints2GlobalCoords = 
					complexGeometryTransform2->applyToCopy(collision.properties.contacts[i].contactPoints2);

				render::Point lightPoint;
				lightPoint.position = contactPoints1GlobalCoords;
				lightPoint.color = render::ColorRGBAf(render::ColorRGBf::MAGENTA());
				render::drawCross(renderer, lightPoint, 0.125f);

				lightPoint.position = contactPoints2GlobalCoords;
				lightPoint.color = render::ColorRGBAf(render::ColorRGBf::CYAN());
				render::drawCross(renderer, lightPoint, 0.125f);
			}
		}
	}
}