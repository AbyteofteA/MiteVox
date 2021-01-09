
#ifndef FILELOADERASYNC_H
#define FILELOADERASYNC_H

#include "loadBytes.h"

#include <filesystem>
namespace fs = std::filesystem;
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
		inline void loadAndParseAsync(std::string _filename, void** _destination, void (*_parseFunction)(std::string filename, void** objectDestination, char* flag) = NULL)
		{
			if (exists(_destination))
			{
				return;
			}

			if (!fs::exists(fs::path(_filename)))
			{
				printf("ERROR: Cannot open %s\n", _filename.c_str());
				return;
			}

			FileLoaderAsyncInfo* fileInfo = new FileLoaderAsyncInfo();
			fileInfo->filename = _filename;
			fileInfo->parseFunction = _parseFunction;

			fileInfo->objectData = NULL;
			fileInfo->destination = _destination;

			if (_parseFunction == NULL) // there is no load/parse function
			{
				fileInfo->loaderThread = std::thread(loadBytes, fileInfo->filename, &fileInfo->objectData, &fileInfo->fileStatus);
			}
			else // there is a load/parse function
			{
				fileInfo->loaderThread = std::thread(fileInfo->parseFunction, fileInfo->filename, &fileInfo->objectData, &fileInfo->fileStatus);
			}
			fileRecords.push_back(fileInfo);
		}

		/*****************************************************************************************
		Checks if any of the files is loaded/parsed and updates the file status.
		*****************************************************************************************/
		inline void update()
		{
			for (int i = 0; i < (int)fileRecords.size(); i++)
			{
				if (fileRecords[i]->fileStatus == READY)
				{
					fileRecords[i]->loaderThread.join();
					*fileRecords[i]->destination = fileRecords[i]->objectData;
					fileRecords.erase(fileRecords.begin() + i);
					i--;
				}
			}
		}

		/*****************************************************************************************
		Checks if the object is loaded/parsed.
		*****************************************************************************************/
		inline bool exists(void** object)
		{
			for (auto fileRecord : fileRecords)
			{
				if (fileRecord->destination == object)
				{
					return true;
				}
			}
			return false;
		}

		/*****************************************************************************************
		Awaits until all files are read and parsed.
		*****************************************************************************************/
		inline void awaitAll()
		{
			while (!fileRecords.empty())
			{
				update();
			}
		}

	};

	extern FileLoaderAsync fileLoader;
}

#endif