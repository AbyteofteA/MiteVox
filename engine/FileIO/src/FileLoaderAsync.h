
#ifndef FILELOADERASYNC_H
#define FILELOADERASYNC_H

#include <string>
#include <thread>
#include <vector>

namespace fileio
{
	struct FileLoaderAsyncInfo
	{
		std::string filename;

#define ERR -1
#define LOADING 0
#define READY 1
		char fileStatus = LOADING;

		std::thread loaderThread;

		void (*parseFunction)(std::string filename, void** objectDestination, char* flag);

		// Temporary storage for the parsed object.
		void* objectData;

		// Destination is assigned with data only when/if the data is ready.
		void** destination;
	};

	struct FileLoaderAsync
	{
		// Vector of files that are currently loaded or parsed.
		std::vector<FileLoaderAsyncInfo*> fileRecords;

		/*****************************************************************************************
		Reads a file from _filename and:
			a) if _parseFunction is passed, parses the file with the function and
		then assigns objectDestination with the pointer to parsed object.
			b) if _parseFunction == NULL, assigns objectDestination with the pointer
		to the read file.
		*****************************************************************************************/
		void loadAndParseAsync(std::string _filename, void** _destination, void (*_parseFunction)(std::string filename, void** objectDestination, char* flag) = NULL);
		
		/*****************************************************************************************
		Checks if any of the files is loaded/parsed and updates the file status.
		*****************************************************************************************/
		inline void update();

		/*****************************************************************************************
		Checks if the object is loaded/parsed.
		*****************************************************************************************/
		inline bool exists(void** object);
		
		/*****************************************************************************************
		Awaits until all files are read and parsed.
		*****************************************************************************************/
		void awaitAll();

	};

	extern FileLoaderAsync fileLoader;
}

#endif