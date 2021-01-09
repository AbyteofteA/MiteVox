
#ifndef MITEVOX_SETTINGS_H
#define MITEVOX_SETTINGS_H

#include <string>

namespace mitevox
{
	struct EngineSettings
	{
		std::string executionPath = "";
		std::string resourcePath = "";
		std::string configPath = "";

		// Default period settings.

		double physicsPeriod = 0.06;
		double rendererPeriod = 0.0;


	};


}

#endif