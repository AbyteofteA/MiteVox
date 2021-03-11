
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <thread>

/*

Log format:
	[TIMESTAMP] EVENTNAME STATUS: "MESSAGE" ID PID

	TIMESTAMP - [YYYY-MM-DD hh:mm:ss uuuuuu] or just microseconds
	ID - the thread ID
	PID - parent thread ID
	EVENTNAME
	STATUS = { INFO, WARNING, ERROR, START, END }

    {
      log:
      [
        { time: "YYYY-MM-DD hh:mm:ss uuuuuu", name: "main", status: "INFO", text: "Hello, world!", id: 789456 },
        { time: "YYYY-MM-DD hh:mm:ss uuuuuu", name: "main", status: "INFO", text: "Hello, world!", id: 789456 },
        { time: "YYYY-MM-DD hh:mm:ss uuuuuu", name: "main", status: "INFO", text: "Hello, world!", id: 789456 }
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

		inline std::string info(std::string name, std::string message);
		inline std::string warning(std::string name, std::string message);
		inline std::string error(std::string name, std::string message);

	private:

		bool logConsole;
		bool logfile;
		std::string filePath;

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
		std::string infoStr =
			"{ time: \"" + getTimeStamp() +
			"\", status: \"INFO" +
			"\", text: \"" + message +
			"\", name: \"" + name +
			"\", id: " + getTreadID() + " }";

		if (logConsole)
		{
			std::cout << infoStr << std::endl;
		}
		if (logfile)
		{

		}

		return infoStr;
	}

	inline std::string Logger::warning(std::string name, std::string message)
	{
		std::string warningStr =
			"{ time: \"" + getTimeStamp() +
			"\", status: \"WARNING" +
			"\", text: \"" + message +
			"\", name: \"" + name +
			"\", id: " + getTreadID() + " }";

		if (logConsole)
		{
			std::cout << warningStr << std::endl;
		}
		if (logfile)
		{

		}

		return warningStr;
	}

	inline std::string Logger::error(std::string name, std::string message)
	{
		std::string errorStr =
			"{ time: \"" + getTimeStamp() +
			"\", status: \"ERROR" +
			"\", text: \"" + message +
			"\", name: \"" + name +
			"\", id: " + getTreadID() + " }";

		if (logConsole)
		{
			std::cout << errorStr << std::endl;
		}
		if (logfile)
		{

		}

		return errorStr;
	}

	inline std::string Logger::getTreadID()
	{
		std::stringstream sstream;
		sstream << std::this_thread::get_id();
		return sstream.str();
	}

	// BUG: it doesn't return appropriate timestamp
	inline std::string Logger::getTimeStamp()
	{
		time_t now = time(nullptr);
		char date[64];
		ctime_s(date, sizeof(date), &now);

		std::string timeStamp = std::string(date);
		timeStamp.pop_back();
		return timeStamp;
	}
}

#endif