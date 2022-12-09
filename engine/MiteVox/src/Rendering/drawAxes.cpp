#include "drawAxes.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

namespace mitevox
{
	void drawAxes(render::RendererSettings* renderer)
	{
		render::ColorRGBAf whiteColor = { 1.0, 1.0, 1.0, 1.0 };
		render::ColorRGBAf redColor = { 1.0, 0.0, 0.0, 1.0 };
		render::ColorRGBAf greenColor = { 0.0, 1.0, 0.0, 1.0 };
		render::ColorRGBAf blueColor = { 0.0, 0.0, 1.0, 1.0 };

		render::Point p1, p2, p3, p4;
		p1.color = whiteColor;
		p1.position = { 0.0, 0.0, 0.0 };
		p2.color = redColor;
		p2.position = { 1.0, 0.0, 0.0 };
		p3.color = greenColor;
		p3.position = { 0.0, 1.0, 0.0 };
		p4.color = blueColor;
		p4.position = { 0.0, 0.0, 1.0 };
		render::drawLine(renderer, p1, p2);
		render::drawLine(renderer, p1, p3);
		render::drawLine(renderer, p1, p4);
	}
}
