
#ifndef MITEVOX_SETTINGS_H
#define MITEVOX_SETTINGS_H

#include <string>

namespace mitevox
{
	struct EngineSettings
	{
		int debug = 0;

		std::string executionPath = "";
		std::string resourcePath = "";
		std::string configPath = "";

		// Default period settings.

		double physicsPeriod = 0.06;
		double rendererPeriod = 0.0;

		void fromJSON(fileio::JSON* json)
		{
			debug = (int)json->getNumber("debug");
			resourcePath = fs::path(json->getString("resource_path")).string();
			configPath = fs::path(json->getString("config_path")).string();
			physicsPeriod = json->getNumber("physics_period");
			rendererPeriod = json->getNumber("renderer_period");
		}
		fileio::JSON* toJSON()
		{
			fileio::JSON* json = new fileio::JSON();

			json->setField("debug", (double)debug);
			json->setField("resource_path", resourcePath);
			json->setField("config_path", configPath);
			json->setField("physics_period", physicsPeriod);
			json->setField("renderer_period", rendererPeriod);

			return json;
		}
	};


}

#endif