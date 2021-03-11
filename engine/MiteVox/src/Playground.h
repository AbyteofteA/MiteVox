
#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include "EngineSettings.h"
#include "Scene.h"

#include <unordered_map>

namespace mitevox
{
	class Playground
	{
	public:

		Playground();
		~Playground();

		unsigned int createScene(std::string name = "Untitled");
		void deleteScene(unsigned int ID);

		Scene* getActiveScene();

	private:

		unsigned int activeScene = 0;
		std::unordered_map<unsigned int, Scene*> scenes;
		mathem::UniqueIDGenerator<unsigned int> IDGenerator;
	};
}

#endif