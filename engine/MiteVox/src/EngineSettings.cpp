#include "EngineSettings.h"

#include "engine/CodeSafety/src/ensureValue.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Profiler/src/Logger.h"
#include "engine/UIEventHandler/src/InputHandler.h"
#include "engine/Renderer/src/RendererAPI/RendererSettings.h"

#include <string>

namespace mitevox
{
	EngineSettings::EngineSettings(std::string _executionPath)
	{
		executionDir = _executionPath;

		fileio::JSON* engineConfig = new fileio::JSON();
		engineConfig->readFromFile(executionDir + "\\engine_config.json");

		fromJSON(engineConfig);

		InputHandler::init(renderer->getWindow());

		delete engineConfig;
	}

	EngineSettings::~EngineSettings()
	{
		if (renderer)
		{
			render::closeRenderer(renderer);
			delete renderer;
		}
	}

	void EngineSettings::fromJSON(fileio::JSON* json)
	{
		fileio::JSON* generalConfig = json->getFieldObject("general");
		fileio::JSON* loggingConfig = json->getFieldObject("logging");
		fileio::JSON* pathsConfig = json->getFieldObject("paths");
		fileio::JSON* rendererConfig = json->getFieldObject("renderer");

		debug = generalConfig->getFieldBooleanOrDefault("debug", true);
		setCleanupPeriod(generalConfig->getFieldNumberOrDefault("cleanup_period", 0.5f));
		setPhysicsPeriod(generalConfig->getFieldNumberOrDefault("physics_period", 0.06f));
		setRendererPeriod(generalConfig->getFieldNumberOrDefault("renderer_period", 0.017f));

		fs::path _executionPath = fs::path(executionDir);
		fs::current_path(_executionPath);

		logConsole = loggingConfig->getFieldBooleanOrDefault("log_console", true);
		logFile = loggingConfig->getFieldBooleanOrDefault("log_file", true);
		fs::path _logPath = fs::path(loggingConfig->getFieldStringOrDefault("log_dir", ""));
		logDir = fs::relative(_logPath, _executionPath).string();

		fs::path _configPath = fs::path(pathsConfig->getFieldStringOrDefault("config_dir", ""));
		configDir = fs::relative(_configPath, _executionPath).string();
		fs::path _resourcePath = fs::path(pathsConfig->getFieldStringOrDefault("resource_dir", ""));
		resourceDir = fs::relative(_resourcePath, _executionPath).string();
		fs::path _savesPath = fs::path(pathsConfig->getFieldStringOrDefault("saves_dir", ""));
		_savesPath = fs::relative(_savesPath, _executionPath).string();

		logger = profile::Logger(profile::LoggerMode::LOG_IN_CONSOLE, logDir);

		int screenWidth = (int)rendererConfig->getFieldNumberOrDefault("screen_width", 720.0f);
		int screenHeight = (int)rendererConfig->getFieldNumberOrDefault("screen_height", 480.0f);
		bool backfaceCulling = rendererConfig->getFieldBooleanOrDefault("back_culling", true);

		// TODO: move clearColor to engine_config.json .
		renderer = render::initRenderer(screenWidth, screenHeight, false, backfaceCulling, { 0.05f, 0.05f, 0.05f });
		if (renderer)
		{
			logger.logInfo("EngineSettings", "Window is created.");
			logger.logInfo("EngineSettings", "Vendor: " + render::getVendorName());
			logger.logInfo("EngineSettings", "Renderer: " + render::getRendererName());
			logger.logInfo("EngineSettings", "Version: " + render::getVersion());
			logger.logInfo("EngineSettings", "Language Version: " + render::getLanguageVersion());
		}
		else
		{
			logger.logError("EngineSettings", "Cannot create window!");
		}
	}

	fileio::JSON* EngineSettings::toJSON()
	{
		fileio::JSON* json = new fileio::JSON();

		fileio::JSON* generalConfig = json->setFieldType("general", fileio::JSONtype::OBJECT);
		fileio::JSON* loggingConfig = json->setFieldType("logging", fileio::JSONtype::OBJECT);
		fileio::JSON* pathsConfig = json->setFieldType("paths", fileio::JSONtype::OBJECT);
		fileio::JSON* rendererConfig = json->setFieldType("renderer", fileio::JSONtype::OBJECT);

		generalConfig->setField("debug", debug);
		generalConfig->setField("cleanup_period", cleanupPeriod);
		generalConfig->setField("physics_period", physicsPeriod);
		generalConfig->setField("renderer_period", rendererPeriod);

		loggingConfig->setField("log_console", logConsole);
		loggingConfig->setField("log_file", logFile);
		loggingConfig->setField("log_dir", logDir);

		pathsConfig->setField("config_dir", configDir);
		pathsConfig->setField("resource_dir", resourceDir);

		rendererConfig->setField("screen_width", (double)renderer->screenWidth);
		rendererConfig->setField("screen_height", (double)renderer->screenHeight);
		rendererConfig->setField("back_culling", (bool)renderer->backfaceCulling);

		return json;
	}

	std::string EngineSettings::getLogDir()
	{
		return logDir;
	}

	std::string EngineSettings::getExecutionDir()
	{
		return executionDir;
	}

	std::string EngineSettings::getResourceDir()
	{
		return resourceDir;
	}

	std::string EngineSettings::getConfigDir()
	{
		return configDir;
	}

	float EngineSettings::getCleanupPeriod()
	{
		return cleanupPeriod;
	}

	void EngineSettings::setCleanupPeriod(float value)
	{
		if (value > 10.0f || value < 0.0f)
		{
			cleanupPeriod = 0.5f;
		}
		else
		{
			cleanupPeriod = value;
		}
	}

	float EngineSettings::getPhysicsPeriod()
	{
		return physicsPeriod;
	}

	void EngineSettings::setPhysicsPeriod(float value)
	{
		if (value > 0.5f || value < 0.0f)
		{
			physicsPeriod = 0.06f;
		}
		else
		{
			physicsPeriod = value;
		}
	}

	float EngineSettings::getAnimationsPeriod()
	{
		return animationPeriod;
	}

	void EngineSettings::setAnimationsPeriod(float value)
	{
		if (value > 0.5f || value < 0.0f)
		{
			animationPeriod = 0.06f;
		}
		else
		{
			animationPeriod = value;
		}
	}

	float EngineSettings::getRendererPeriod()
	{
		return rendererPeriod;
	}

	void EngineSettings::setRendererPeriod(float value)
	{
		if (value > 0.1f || value < 0.0f)
		{
			rendererPeriod = 0.0f;
		}
		else
		{
			rendererPeriod = value;
		}
	}

	float EngineSettings::getEqualityTolerance()
	{
		return equalityTolerance;
	}

	void EngineSettings::setEqualityTolerance(float value)
	{
		equalityTolerance = safety::ensureRange(value, 0.0000001f, 0.01f);
	}

	render::RendererSettings* EngineSettings::getRendererSettings()
	{
		return renderer;
	}
}