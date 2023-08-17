#ifndef LOGGER_H
#define LOGGER_H

#include <string>

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
		Logger();

		/// <summary>
		/// Constructor for the file logger. 
		/// </summary>
		Logger(LoggerMode mode, std::string _filePath);

		void logInfo(std::string name, std::string message);
		void logWarning(std::string name, std::string message);
		void logError(std::string name, std::string message);

		/// <summary>
		/// Logs start of some process.
		/// </summary>
		void logStart(std::string name);

		/// <summary>
		/// Logs end of some process.
		/// </summary>
		void logEnd(std::string name);

	private:

		LoggerMode _mode;
		std::string filePath;
		
		std::string formConsoleLog(std::string name, std::string status, std::string message);
		std::string formJSONLog(std::string name, std::string status, std::string message);
		void placeLog(std::string name, std::string status, std::string message);
		
		std::string getTreadID();
		std::string getTimeStamp();
	};
}

#endif