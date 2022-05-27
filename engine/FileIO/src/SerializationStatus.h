
#ifndef PARSESTATUS_H
#define PARSESTATUS_H

namespace fileio
{
	enum struct SerializationStatus
	{
		ERROR_UNKNOWN = -1,
		PENDING,
		COMPLETE
	};

	enum struct DeserializationStatus
	{
		ERROR_UNKNOWN = -1,
		PENDING,
		COMPLETE
	};
}

#endif