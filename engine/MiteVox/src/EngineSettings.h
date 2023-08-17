#ifndef MITEVOX_SETTINGS_H
#define MITEVOX_SETTINGS_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/MiteVox/src/Profiler/Logger.h"
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
		bool debug = true;
		bool spaceCulling = true; // TODO: add to EngineSettings

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

		float getEqualityTolerance();
		void setEqualityTolerance(float value);

		render::RendererSettings* getRendererSettings();

	private:

		float cleanupPeriod = 0.5f;
		float physicsPeriod = 0.06f;
		float animationPeriod = 0.06f;
		float rendererPeriod = 0.017f;

		bool logConsole = true;
		bool logFile = true;
		std::string logDir = "";

		std::string executionDir = "";
		std::string resourceDir = "";
		std::string configDir = "";
		std::string savesDir = "";

		float equalityTolerance = 0.001f; // TODO: add to EngineSettings

		render::RendererSettings* renderer = nullptr;
	};


}

#endif