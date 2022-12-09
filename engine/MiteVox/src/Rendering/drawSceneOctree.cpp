#include "drawSceneOctree.h"

#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/Math/src/DataStructures/Graphs/Octree.h"
#include "drawCollider.h"

namespace mitevox
{
	void drawOctreeNodeRecursive(render::RendererSettings* renderer, mathem::OctreeNode<Node>* octreeNode)
	{
		drawCollider(renderer, &octreeNode->boundingBox, mathem::GeometryTransform());

		for (size_t i = 0; i < 8; ++i)
		{
			if (octreeNode->subNodes[i] != nullptr)
			{
				drawOctreeNodeRecursive(renderer, octreeNode->subNodes[i]);
			}
		}
	}

	void drawSceneOctree(render::RendererSettings* renderer, mathem::Octree<Node>* octree)
	{
		//drawOctreeNodeRecursive(renderer, octree->origin);
	}
}