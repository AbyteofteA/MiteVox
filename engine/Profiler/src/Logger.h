
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>

/*

Log format:
	[TIMESTAMP] EVENT STATUS: "MESSAGE" ID PID
{
	log:
	[
		{time: 123456, event: "main", status: "INFO", text: "Hello, world!", id: 789456, pid: 789456},
		{time: 234567, event: "main", status: "INFO", text: "Hello, world!", id: 789456, pid: 789456},
		{time: 345678, event: "main", status: "INFO", text: "Hello, world!", id: 789456, pid: 789456}
	]
}
	TIMESTAMP - [YYYY-MM-DD hh:mm:ss uuuuuu] or just microseconds
	ID - the thread ID
	PID - parent thread ID
	EVENT
	STATUS = { INFO, WARNING, ERROR, START, END }
	
*/

namespace profile
{
	class Logger
	{
		bool logConsole;
		bool logfile;
		std::string filePath;

		std::chrono::high_resolution_clock::time_point startTime;

	public:

		Logger()
		{
			startTime =
				std::chrono::high_resolution_clock::now();

			logConsole = true;
			logfile = false;
		}
		Logger(bool _logConsole, std::string _filePath)
		{
			startTime =
				std::chrono::high_resolution_clock::now();

			logConsole = _logConsole;
			logfile = true;
			filePath = _filePath;
		}

		void info(std::string message)
		{
			std::string infoStr =
				"[" + getTimeStamp() + "] INFO: \"" + message + "\"";

			if (logConsole)
			{
				std::cout << infoStr << std::endl;
			}
			if (logfile)
			{
				
			}
		}
		void warning(std::string message)
		{
			std::string warningStr =
				"[" + getTimeStamp() + "] WARNING: \"" + message + "\"";

			if (logConsole)
			{
				std::cout << warningStr << std::endl;
			}
			if (logfile)
			{

			}
		}
		void error(std::string message)
		{
			std::string errorStr =
				"[" + getTimeStamp() + "] ERROR: \"" + message + "\"";

			if (logConsole)
			{
				std::cout << errorStr << std::endl;
			}
			if (logfile)
			{

			}
		}

		std::string getTreadID()
		{
			std::stringstream sstream;
			sstream << std::this_thread::get_id();
			return sstream.str();
		}
		std::string getTimeStamp()
		{
			auto now = std::chrono::high_resolution_clock::now();
			long long time =
				std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();
			return std::to_string(time);
		}
	};
}

#endif