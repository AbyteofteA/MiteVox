
#include "loadBytes.h"

#include "FileLoaderAsync.h"

namespace fileio
{
	FileLoaderAsync fileLoader;

	void FileLoaderAsync::loadAndParseAsync(std::string _filename, void** _destination, void (*_parseFunction)(std::string filename, void** objectDestination, char* flag))
	{
		if (exists(_destination))
		{
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
	void FileLoaderAsync::update()
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
	bool FileLoaderAsync::exists(void** object)
	{
		for (int i = 0; i < (int)fileRecords.size(); i++)
		{
			if (fileRecords[i]->destination == object)
			{
				return true;
			}
		}
		return false;
	}

	/*****************************************************************************************
	Awaits until all files are read and parsed.
	*****************************************************************************************/
	void FileLoaderAsync::awaitAll()
	{
		while (!fileRecords.empty())
		{
			update();
		}
	}
}