#include "drawSceneOctree.h"

#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/Math/src/DataStructures/Graphs/Octree.h"
#include "drawGeometryPrimitive.h"

namespace mitevox
{
	void drawOctreeNodeRecursive(render::RendererSettings* renderer, mathem::OctreeNode<Entity*>* octreeNode, const size_t nodeCapacity)
	{
		mathem::GeometryTransform zeroTransform;

		if (octreeNode->getOverloadCount(nodeCapacity) != 0)
		{
			drawGeometryPrimitive(renderer, &octreeNode->boundingBox, &zeroTransform, render::ColorRGBAf(render::ColorRGBf::MAGENTA()));
			
		}
		else if (octreeNode->dataPoints.getElementsCount() == 0)
		{
			drawGeometryPrimitive(renderer, &octreeNode->boundingBox, &zeroTransform, render::ColorRGBAf(render::ColorRGBf::YELLOW()));
		}
		else
		{
			drawGeometryPrimitive(renderer, &octreeNode->boundingBox, &zeroTransform, render::ColorRGBAf(render::ColorRGBf::WHITE()));
		}
		//drawGeometryPrimitive(renderer, &octreeNode->boundingBox, &zeroTransform, whiteColor);
		for (size_t i = 0; i < 8; ++i)
		{
			if (octreeNode->subNodes[i] != nullptr)
			{
				drawOctreeNodeRecursive(renderer, octreeNode->subNodes[i], nodeCapacity);
			}
		}
	}

	void drawSceneOctree(render::RendererSettings* renderer, mathem::Octree<Entity*>* octree)
	{
		drawOctreeNodeRecursive(renderer, octree->origin, octree->getNodeCapacity());
	}
}