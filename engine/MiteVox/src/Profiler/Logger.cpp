#include "Logger.h"

#include <iostream>
#include <sstream>
#include <ctime>
#include <thread>

namespace profile
{
	Logger::Logger()
	{
		_mode = LoggerMode::LOG_IN_CONSOLE;
	}

	Logger::Logger(LoggerMode mode, std::string _filePath)
	{
		_mode = mode;
		filePath = _filePath;
	}

	void Logger::logInfo(std::string name, std::string message)
	{
		placeLog(name, "INFO   ", message);
	}

	void Logger::logWarning(std::string name, std::string message)
	{
		placeLog(name, "WARNING", message);
	}

	void Logger::logError(std::string name, std::string message)
	{
		placeLog(name, "ERROR  ", message);
	}

	void Logger::logStart(std::string name)
	{
		placeLog(name, "START  ", "");
	}

	void Logger::logEnd(std::string name)
	{
		placeLog(name, "END    ", "");
	}

	std::string Logger::formConsoleLog(std::string name, std::string status, std::string message)
	{
		std::string log;
		log += getTimeStamp();
		log += " | ";
		log += status;
		log += " | ";
		log += name;
		log += " | ";
		log += message;
		log += " | ";
		log += getTreadID();

		return log;
	}

	std::string Logger::formJSONLog(std::string name, std::string status, std::string message)
	{
		std::string log;
		log += "{ time: \"";
		log += getTimeStamp();
		log += "\", status: \"";
		log += status;
		log += "\", name: \"";
		log += name;
		log += "\", text: \"";
		log += message;
		log += "\", id: ";
		log += getTreadID();
		log += " }";

		return log;
	}

	void Logger::placeLog(std::string name, std::string status, std::string message)
	{
		if ((int)_mode & (int)LoggerMode::LOG_IN_CONSOLE)
		{
			std::cout << formConsoleLog(name, status, message) << std::endl;
		}
		if ((int)_mode & (int)LoggerMode::LOG_IN_JSON_FILE)
		{
			// TODO: Implement file logging.
		}
	}

	std::string Logger::getTreadID()
	{
		std::stringstream sstream;
		sstream << std::this_thread::get_id();
		return sstream.str();
	}

	std::string Logger::getTimeStamp()
	{
		std::timespec tmspec;
		std::timespec_get(&tmspec, TIME_UTC);

		char calendarTimeBuffer[64];
		char resultBuffer[128];

		std::tm calendarTime;
		localtime_s(&calendarTime, &tmspec.tv_sec);

		strftime(calendarTimeBuffer, sizeof(calendarTimeBuffer), "%F %T", &calendarTime);
		sprintf_s(resultBuffer, "%s.%09ld\n", calendarTimeBuffer, tmspec.tv_nsec);

		std::string timeStamp = std::string(resultBuffer);
		timeStamp.pop_back();
		return timeStamp;
	}
}