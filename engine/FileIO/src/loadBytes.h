
#ifndef LOADBYTES_H
#define LOADBYTES_H

#include "FileStatus.h"
#include <string>

namespace fileio
{
	/*****************************************************************************************
	Reads a file from filename and assigns the result with the pointer to the read file.
	Assignes the flag with 1 when the file is loaded.
	\return Size of the file if success, -1 otherwise.
	*****************************************************************************************/
	inline void loadBytes(std::string filename, void** result, FileStatus* flag)
	{
		*result = nullptr;
		*flag = FileStatus::LOADING;
		char* fileData = nullptr;

		FILE* file;
		fopen_s(&file, filename.c_str(), "r");
		if (file == nullptr)
		{
			printf("\n ERROR! Cannot open the file.\n");
			printf("File: %s\n", filename.c_str());
			*flag = FileStatus::ERROR;
			return;
		}
		else // read the file
		{
			// Determine the file size.
			int size = 1;
			fseek(file, 0, SEEK_END);
			size = ftell(file) + 1;
			fseek(file, 0, SEEK_SET);

			fileData = (char*)malloc(sizeof(char) * size);

			char tmp = 0;
			size = 0;
			while ((tmp = fgetc(file)) != EOF)
			{
				fileData[size] = tmp;
				size++;
			}
			fileData[size - 1] = '\0';

			fclose(file);
			(*result) = (void*)fileData;

			*flag = FileStatus::READY;
			return;
		}
	}
}

#endif