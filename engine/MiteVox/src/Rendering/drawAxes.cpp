#include "drawAxes.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

namespace mitevox
{
	void drawAxes(render::RendererSettings* renderer)
	{
		render::ColorRGBAf redColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		render::ColorRGBAf greenColor = { 0.0f, 1.0f, 0.0f, 1.0f };
		render::ColorRGBAf blueColor = { 0.0f, 0.0f, 1.0f, 1.0f };

		render::Point p1, p2, p3, p4;
		p1.position = { 0.0f, 0.0f, 0.0f };
		p2.color = redColor;
		p2.position = { 1.0f, 0.0f, 0.0f };
		p3.color = greenColor;
		p3.position = { 0.0f, 1.0f, 0.0f };
		p4.color = blueColor;
		p4.position = { 0.0f, 0.0f, 1.0f };

		p1.color = redColor;
		render::drawLine(renderer, p1, p2);

		p1.color = greenColor;
		render::drawLine(renderer, p1, p3);

		p1.color = blueColor;
		render::drawLine(renderer, p1, p4);
	}
}
