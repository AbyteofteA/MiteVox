#ifndef MITEVOX_SETTINGS_H
#define MITEVOX_SETTINGS_H

#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/MiteVox/src/Profiler/Logger.h"
#include "engine/MiteVox/src/Physics/PhysicsSolverType.h"
#include "engine/UIEventHandler/src/InputHandler.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"

#include <string>

namespace mitevox
{
	class EngineSettings
	{
		friend class Engine;
		friend class MiteVoxAPI;

	public:

		profile::Logger logger;
		bool debug = true;
		bool spaceCulling = true; // TODO: add to EngineSettings

		explicit EngineSettings(std::string executableDir);

		void fromJSON(fileio::JSON* json);
		fileio::JSON* toJSON();

		// Getters //

		std::string getLogDir();
		std::string getExecutableDir();
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

		render::RendererSettings& getRendererSettings();

	private:

		float cleanupPeriod = 0.5f;
		float physicsPeriod = 0.06f;
		float animationPeriod = 0.06f;
		float rendererPeriod = 0.017f;

		bool logConsole = true;
		bool logFile = true;
		std::string logDir = "";

		std::string executableDir = "";
		std::string resourceDir = "";
		std::string configDir = "";
		std::string savesDir = "";

		float equalityTolerance = 0.001f; // TODO: add to EngineSettings

		struct
		{
			PhysicsSolverType physicsSolver = PhysicsSolverType::POSITION_BASED;
			size_t substepsCount = 1;
			float sleepTime = 0.333f;
			float maxLinearSpeed = -1.0f;
			float maxAngularSpeed = 10.0f;
			float linearDamping = 0.01f;	// %/sec
			float angularDamping = 0.01f;	// %/sec
			float linearSleepThreshold = 0.075f;
			float angularSleepThreshold = 0.075f;
		} physics;

		render::RendererSettings renderer;
	};


}

#endif