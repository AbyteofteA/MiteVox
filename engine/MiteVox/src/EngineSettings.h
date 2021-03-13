
#ifndef MITEVOX_SETTINGS_H
#define MITEVOX_SETTINGS_H

#include "engine/FileIO/src/FileIO.h"
#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Profiler/src/Logger.h"
#include "engine/UIEventHandler/src/InputHandler.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"

#include <string>

namespace mitevox
{
	class EngineSettings
	{
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

		double getCleanupPeriod();
		void setCleanupPeriod(double value);

		double getPhysicsPeriod();
		void setPhysicsPeriod(double value);

		double getRendererPeriod();
		void setRendererPeriod(double value);

		render::RendererSettings* getRendererSettings();
		InputHandler* getInputHandler();

	private:

		bool debug = true;
		double cleanupPeriod = 0.5;
		double physicsPeriod = 0.06;
		double rendererPeriod = 0.0;

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