
#ifndef PARSESTATUS_H
#define PARSESTATUS_H

namespace fileio
{
	enum struct SerializationStatus
	{
		ERROR = -1,
		PENDING,
		COMPLETE
	};

	enum struct DeserializationStatus
	{
		ERROR = -1,
		PENDING,
		COMPLETE
	};
}

#endif