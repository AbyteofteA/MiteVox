
#ifndef MITEVOX_ENGINE_H
#define MITEVOX_ENGINE_H

#include "EngineSettings.h"
#include "Playground.h"

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

		void run();

	private:

		Playground* playground;
	};
}

#endif