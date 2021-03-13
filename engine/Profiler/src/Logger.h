
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
	    { time: "YYYY-MM-DD hh:mm:ss.uuuuuuuuu", name: "main", status: "INFO", text: "Hello, world!", id: 789456 },
	    { time: "YYYY-MM-DD hh:mm:ss.uuuuuuuuu", name: "main", status: "INFO", text: "Hello, world!", id: 789456 },
	    { time: "YYYY-MM-DD hh:mm:ss.uuuuuuuuu", name: "main", status: "INFO", text: "Hello, world!", id: 789456 }
	  ]
	}
*/

namespace profile
{
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
		inline Logger(bool _logConsole, std::string _filePath);

		/// <summary>
		/// Logs info message.
		/// </summary>
		inline std::string info(std::string name, std::string message);

		/// <summary>
		/// Logs warning message.
		/// </summary>
		inline std::string warning(std::string name, std::string message);

		/// <summary>
		/// Logs error message.
		/// </summary>
		inline std::string error(std::string name, std::string message);

		/// <summary>
		/// Logs start of some process.
		/// </summary>
		inline std::string start(std::string name);

		/// <summary>
		/// Logs end of some process.
		/// </summary>
		inline std::string end(std::string name);

	private:

		bool logConsole;
		bool logfile;
		std::string filePath;

		inline std::string formLog(std::string name, std::string status, std::string message);
		inline void placeLog(std::string log);

		inline std::string getTreadID();
		inline std::string getTimeStamp();
	};


	// IMPLEMENTATION BELOW //


	inline Logger::Logger()
	{
		logConsole = true;
		logfile = false;
	}

	inline Logger::Logger(bool _logConsole, std::string _filePath)
	{
		logConsole = _logConsole;
		logfile = true;
		filePath = _filePath;
	}

	inline std::string Logger::info(std::string name, std::string message)
	{
		std::string infoStr = formLog(name, "INFO", message);
		placeLog(infoStr);

		return infoStr;
	}

	inline std::string Logger::warning(std::string name, std::string message)
	{
		std::string warningStr = formLog(name, "WARNING", message);
		placeLog(warningStr);

		return warningStr;
	}

	inline std::string Logger::error(std::string name, std::string message)
	{
		std::string errorStr = formLog(name, "ERROR", message);
		placeLog(errorStr);

		return errorStr;
	}

	inline std::string Logger::start(std::string name)
	{
		std::string startStr = formLog(name, "START", "");
		placeLog(startStr);

		return startStr;
	}

	inline std::string Logger::end(std::string name)
	{
		std::string endStr = formLog(name, "END", "");
		placeLog(endStr);

		return endStr;
	}

	inline std::string Logger::formLog(std::string name, std::string status, std::string message)
	{
		std::string log =
			"time: \"" + getTimeStamp() +
			"\", name: \"" + name +
			"\", status: \"" + status +
			"\", text: \"" + message +
			"\", id: " + getTreadID();

		return log;
	}

	inline void Logger::placeLog(std::string log)
	{
		if (logConsole)
		{
			std::cout << log << std::endl;
		}
		if (logfile)
		{
			log = "{ " + log + " }";

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