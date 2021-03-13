
#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include "Scene.h"
#include "EngineSettings.h"
#include "engine/Math/src/Generators/UniqueIDGenerator.h"

#include <unordered_map>
#include <string>

namespace mitevox
{
	class Playground
	{
	public:

		std::string name;
		unsigned int activeScene = 0;

		Playground(std::string _name = "Untitled");
		~Playground();

		void update();
		unsigned int createScene(std::string name, EngineSettings* settings);
		unsigned int createMainScene(std::string name, EngineSettings* settings);
		void deleteScene(unsigned int ID);
		Scene* getActiveScene();

	private:

		std::unordered_map<unsigned int, Scene*> scenes;
		mathem::UniqueIDGenerator<unsigned int> IDGenerator;
	};
}

#endif