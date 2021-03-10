
#ifndef MITEVOX_ENGINE_H
#define MITEVOX_ENGINE_H

#include "EngineSettings.h"
#include "Scene.h"
#include "engine/UIEventHandler/src/InputHandler.h"

#include <unordered_map>

namespace mitevox
{
	class Engine
	{
	public:

		int activeScene = -1;
		EngineSettings* settings;

		Engine(int argc, char* argv[]);
		~Engine();

		void onCreate();
		void onUpdate();
		void onDestroy();
		
		int createScene(std::string name = "Untitled");
		void deleteScene(int ID);

		Scene* getActiveScene();

		void update();

	private:

		InputHandler* inputHandler = nullptr;
		std::unordered_map<int, Scene*> scenes;
		mathem::UniqueIDGenerator<int> IDGenerator;
	};
}

#endif