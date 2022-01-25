
#ifndef FILESTATUS_H
#define FILESTATUS_H

namespace fileio
{
	enum struct FileStatus 
	{ 
		ERROR = -1, 
		LOADING, 
		SAVING, 
		READY 
	};
}

#endif