#ifndef DRAWLIGHTSOURCE_H
#define DRAWLIGHTSOURCE_H

#include "engine/Renderer/src/RendererAPI/RendererSettings.h"
#include "engine/MiteVox//src/Playground/Node.h"

namespace mitevox
{
	void drawLightSource(render::RendererSettings* renderer, Node* lightNode, mathem::GeometryTransform* nodeGlobalTransform);
}

#endif