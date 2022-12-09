#ifndef DRAWSCENEOCTREE_H
#define DRAWSCENEOCTREE_H

#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/Math/src/DataStructures/Graphs/Octree.h"

namespace mitevox
{
	void drawSceneOctree(render::RendererSettings* renderer, mathem::Octree<Node>* octree);
}

#endif