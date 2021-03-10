
#ifndef MITEVOX_SETTINGS_H
#define MITEVOX_SETTINGS_H

#include "engine/FileIO/src/FileIO.h"
#include "engine/Renderer/src/RendererAPI/RendererAPI.h"

#include <string>

namespace mitevox
{
	class EngineSettings
	{
		friend class Engine;
		friend class Scene;

	public:

		EngineSettings();
		EngineSettings(std::string _executionPath);
		~EngineSettings();

		void fromJSON(fileio::JSON* json);
		fileio::JSON* toJSON();

		// Getters //

		std::string getLogDir();
		std::string getExecutionDir();
		std::string getResourceDir();
		std::string getConfigDir();
		render::RendererSettings* getRendererSettings();

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
	};


}

#endif