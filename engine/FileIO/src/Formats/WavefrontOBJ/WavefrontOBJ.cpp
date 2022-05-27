#include "WavefrontOBJ.h"

#include "WavefrontOBJRegex.h"
#include "engine/CodeSafety/src/SafeArray.h"

namespace fileio
{
    WavefrontOBJ::WavefrontOBJ()
    {

    }

    WavefrontOBJ::~WavefrontOBJ()
    {

    }

    SerializationStatus WavefrontOBJ::serialize(safety::SafeByteArray* fileData)
    {

        return SerializationStatus::ERROR_UNKNOWN;
    }

    DeserializationStatus WavefrontOBJ::deserialize(safety::SafeByteArray* fileData)
    {

        return DeserializationStatus::ERROR_UNKNOWN;
    }
}