#include "ComponentDataType.h"

namespace mitevox
{
	size_t getDataTypeSize(ComponentDataType dataType)
    {
		switch (dataType)
		{
		case mitevox::BYTE:
			return 1;
		case mitevox::UNSIGNED_BYTE:
			return 1;
		case mitevox::SHORT:
			return 2;
		case mitevox::UNSIGNED_SHORT:
			return 2;
		case mitevox::UNSIGNED_INT:
			return 4;
		case mitevox::FLOAT:
			return 4;
		default:
			break;
		}
		return 0;
    }
}