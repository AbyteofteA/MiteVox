#include "drawAxes.h"

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

namespace mitevox
{
	void drawAxes(render::RendererSettings* renderer)
	{
		render::Point p1, p2, p3, p4;
		p1.position = { 0.0f, 0.0f, 0.0f };
		p2.color = render::ColorRGBAf(render::ColorRGBf::RED());
		p2.position = { 1.0f, 0.0f, 0.0f };
		p3.color = render::ColorRGBAf(render::ColorRGBf::GREEN());
		p3.position = { 0.0f, 1.0f, 0.0f };
		p4.color = render::ColorRGBAf(render::ColorRGBf::BLUE());
		p4.position = { 0.0f, 0.0f, 1.0f };

		p1.color = render::ColorRGBAf(render::ColorRGBf::RED());
		render::drawLine(renderer, p1, p2);

		p1.color = render::ColorRGBAf(render::ColorRGBf::GREEN());
		render::drawLine(renderer, p1, p3);

		p1.color = render::ColorRGBAf(render::ColorRGBf::BLUE());
		render::drawLine(renderer, p1, p4);
	}
}
