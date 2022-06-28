
#ifndef MITEVOX_SETTINGS_H
#define MITEVOX_SETTINGS_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Profiler/src/Logger.h"
#include "engine/UIEventHandler/src/InputHandler.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"

#include <string>

namespace mitevox
{
	class EngineSettings
	{
		friend class Engine;

	public:

		profile::Logger logger;

		EngineSettings(std::string _executionPath);
		~EngineSettings();

		void fromJSON(fileio::JSON* json);
		fileio::JSON* toJSON();

		// Getters //

		std::string getLogDir();
		std::string getExecutionDir();
		std::string getResourceDir();
		std::string getConfigDir();

		float getCleanupPeriod();
		void setCleanupPeriod(float value);

		float getPhysicsPeriod();
		void setPhysicsPeriod(float value);

		float getAnimationsPeriod();
		void setAnimationsPeriod(float value);

		float getRendererPeriod();
		void setRendererPeriod(float value);

		render::RendererSettings* getRendererSettings();
		InputHandler* getInputHandler();

	private:

		bool debug = true;
		bool spaceCulling = false;
		float cleanupPeriod = 0.5;
		float physicsPeriod = 0.06;
		float animationPeriod = 0.06;
		float rendererPeriod = 0.017;

		bool logConsole = true;
		bool logFile = true;
		std::string logDir = "";

		std::string executionDir = "";
		std::string resourceDir = "";
		std::string configDir = "";
		std::string savesDir = "";

		render::RendererSettings* renderer = nullptr;
		InputHandler* inputHandler = nullptr;
	};


}

#endif