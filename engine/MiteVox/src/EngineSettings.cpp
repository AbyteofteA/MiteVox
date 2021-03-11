
#include "EngineSettings.h"

#include "engine/FileIO/src/FileIO.h"
#include "engine/Renderer/src/RendererAPI/RendererAPI.h"
#include "engine/Profiler/src/Logger.h"
#include "engine/UIEventHandler/src/InputHandler.h"

#include <string>
#include <cstdio>

namespace mitevox
{
	EngineSettings::EngineSettings(std::string _executionPath)
	{
		executionDir = _executionPath;

		fileio::JSON* engineConfig = new fileio::JSON();
		engineConfig->parseFile(executionDir + "\\engine_config.json");
		fromJSON(engineConfig);

		inputHandler = new InputHandler(renderer->getWindow());

		delete engineConfig;
	}

	EngineSettings::~EngineSettings()
	{
		if (renderer)
		{
			render::closeRenderer(renderer);
			delete renderer;
		}

		if (inputHandler)
		{
			delete inputHandler;
		}
	}

	void EngineSettings::fromJSON(fileio::JSON* json)
	{
		fileio::JSON* generalConfig = json->getObject("general");
		fileio::JSON* loggingConfig = json->getObject("logging");
		fileio::JSON* pathsConfig = json->getObject("paths");
		fileio::JSON* rendererConfig = json->getObject("renderer");

		debug = generalConfig->getBoolean("debug");
		cleanupPeriod = generalConfig->getNumber("cleanup_period");
		physicsPeriod = generalConfig->getNumber("physics_period");
		rendererPeriod = generalConfig->getNumber("renderer_period");

		fs::path _executionPath = fs::path(executionDir);
		fs::current_path(_executionPath);

		logConsole = loggingConfig->getBoolean("log_console");
		logFile = loggingConfig->getBoolean("log_file");
		fs::path _logPath = fs::path(loggingConfig->getString("log_dir"));
		logDir = fs::relative(_logPath, _executionPath).string();

		fs::path _configPath = fs::path(pathsConfig->getString("config_dir"));
		configDir = fs::relative(_configPath, _executionPath).string();
		fs::path _resourcePath = fs::path(pathsConfig->getString("resource_dir"));
		resourceDir = fs::relative(_resourcePath, _executionPath).string();
		fs::path _savesPath = fs::path(pathsConfig->getString("saves_dir"));
		_savesPath = fs::relative(_savesPath, _executionPath).string();

		logger = profile::Logger(true, logDir);

		int screenWidth = (int)generalConfig->getNumber("screen_width");
		int screenHeight = (int)generalConfig->getNumber("screen_height");
		bool backfaceCulling = generalConfig->getBoolean("back_culling");

		renderer = render::initRenderer(screenWidth, screenHeight, true, backfaceCulling);
		if (renderer)
		{
			logger.info("EngineSettings", "Window is created.");
			logger.info("EngineSettings", "Vendor: " + render::getVendorName());
			logger.info("EngineSettings", "Renderer: " + render::getRendererName());
			logger.info("EngineSettings", "Version: " + render::getVersion());
			logger.info("EngineSettings", "Language Version: " + render::getLanguageVersion());
		}
		else
		{
			logger.error("EngineSettings", "Cannot create window!");
		}
	}

	fileio::JSON* EngineSettings::toJSON()
	{
		fileio::JSON* json = new fileio::JSON();

		fileio::JSON* generalConfig = json->setField("general", fileio::JSONtype::OBJECT);
		fileio::JSON* loggingConfig = json->setField("logging", fileio::JSONtype::OBJECT);
		fileio::JSON* pathsConfig = json->setField("paths", fileio::JSONtype::OBJECT);
		fileio::JSON* rendererConfig = json->setField("renderer", fileio::JSONtype::OBJECT);

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

	render::RendererSettings* EngineSettings::getRendererSettings()
	{
		return renderer;
	}
}