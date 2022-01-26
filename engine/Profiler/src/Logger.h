
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <thread>

/*
	JSON Log format:

	{
	  log:
	  [
	    { time: "YYYY-MM-DD hh:mm:ss.uuuuuuuuu", status: "INFO", name: "main", text: "Hello, world!", id: 789456 },
	    { time: "YYYY-MM-DD hh:mm:ss.uuuuuuuuu", status: "INFO", name: "main", text: "Hello, world!", id: 789456 },
	    { time: "YYYY-MM-DD hh:mm:ss.uuuuuuuuu", status: "INFO", name: "main", text: "Hello, world!", id: 789456 }
	  ]
	}
*/

namespace profile
{
	enum struct LoggerMode
	{
		LOG_IN_CONSOLE = 0b00000001,
		LOG_IN_JSON_FILE = 0b00000010,
		LOG_IN_CONSOLE_AND_JSON_FILE = 0b00000011
	};

	class Logger
	{
	public:

		/// <summary>
		/// Constructor for the console logger.
		/// </summary>
		inline Logger();

		/// <summary>
		/// Constructor for the file logger. 
		/// </summary>
		inline Logger(LoggerMode mode, std::string _filePath);

		/// <summary>
		/// Logs info message.
		/// </summary>
		inline void logInfo(std::string name, std::string message);

		/// <summary>
		/// Logs warning message.
		/// </summary>
		inline void logWarning(std::string name, std::string message);

		/// <summary>
		/// Logs error message.
		/// </summary>
		inline void logError(std::string name, std::string message);

		/// <summary>
		/// Logs start of some process.
		/// </summary>
		inline void logStart(std::string name);

		/// <summary>
		/// Logs end of some process.
		/// </summary>
		inline void logEnd(std::string name);

	private:

		LoggerMode _mode;
		std::string filePath;

		inline std::string formConsoleLog(std::string name, std::string status, std::string message);
		inline std::string formJSONLog(std::string name, std::string status, std::string message);
		inline void placeLog(std::string name, std::string status, std::string message);

		inline std::string getTreadID();
		inline std::string getTimeStamp();
	};


	// IMPLEMENTATION BELOW //


	inline Logger::Logger()
	{
		_mode = LoggerMode::LOG_IN_CONSOLE;
	}

	inline Logger::Logger(LoggerMode mode, std::string _filePath)
	{
		_mode = mode;
		filePath = _filePath;
	}

	inline void Logger::logInfo(std::string name, std::string message)
	{
		placeLog(name, "INFO   ", message);
	}

	inline void Logger::logWarning(std::string name, std::string message)
	{
		placeLog(name, "WARNING", message);
	}

	inline void Logger::logError(std::string name, std::string message)
	{
		placeLog(name, "ERROR  ", message);
	}

	inline void Logger::logStart(std::string name)
	{
		placeLog(name, "START  ", "");
	}

	inline void Logger::logEnd(std::string name)
	{
		placeLog(name, "END    ", "");
	}

	inline std::string Logger::formConsoleLog(std::string name, std::string status, std::string message)
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

	inline std::string Logger::formJSONLog(std::string name, std::string status, std::string message)
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

	inline void Logger::placeLog(std::string name, std::string status, std::string message)
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

	inline std::string Logger::getTreadID()
	{
		std::stringstream sstream;
		sstream << std::this_thread::get_id();
		return sstream.str();
	}

	inline std::string Logger::getTimeStamp()
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

#endif