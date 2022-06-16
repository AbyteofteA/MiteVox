
#ifndef MITEVOX_ENGINE_H
#define MITEVOX_ENGINE_H

#include "EngineSettings.h"
#include "Playground.h"
#include "engine/Renderer/src/RendererAPI/Color.h"

#include <chrono>
#include <unordered_map>

namespace mitevox
{
	class Engine
	{
	public:

		EngineSettings* settings;

		Engine(int argc, char* argv[]);
		~Engine();

		void onCreate();
		void onUpdate();
		void onDestroy();

		void initRenderer(int width, int height, bool isFullScreen, bool backfaceCulling, render::ColorRGBf clearColor);
		void run();

	private:

		std::chrono::high_resolution_clock::time_point prevCycleTime;

		Playground* playground;
	};
}

#endif