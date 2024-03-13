#ifndef MITEVOX_RESOURCE_H
#define MITEVOX_RESOURCE_H

#include <string>

namespace mitevox
{
	template <class T>
	struct Resource
	{
		std::string path;
		T content;
	};
}

#endif

