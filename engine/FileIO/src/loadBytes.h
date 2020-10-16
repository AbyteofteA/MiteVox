
#ifndef LOADBYTES_H
#define LOADBYTES_H

#include <string>

namespace fileio
{
	/*****************************************************************************************
	Reads a file from filename and assigns the result with the pointer to the read file.
	Assignes the flag with 1 when the file is loaded.
	\return Size of the file if success, -1 otherwise.
	*****************************************************************************************/
	void loadBytes(std::string filename, void** result, char* flag);
}

#endif