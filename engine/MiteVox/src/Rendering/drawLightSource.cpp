#include "drawLightSource.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"
#include "drawGeometryPrimitive.h"

namespace mitevox
{
	void drawLightSource(render::RendererSettings* renderer, Node* lightNode, mathem::GeometryTransform* nodeGlobalTransform)
	{
		render::Point lightPoint;
		
		lightPoint.position = nodeGlobalTransform->translation;
		lightPoint.color.r = lightNode->light.pointLight.lightBase.color.r;
		lightPoint.color.g = lightNode->light.pointLight.lightBase.color.g;
		lightPoint.color.b = lightNode->light.pointLight.lightBase.color.b;
		lightPoint.color.a = 1.0f;

		render::drawSnowflake(renderer, lightPoint, 0.1f);
	}
}