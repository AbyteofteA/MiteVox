#include "BufferViewAccessor.h"

namespace mitevox
{
	BufferViewAccessor::BufferViewAccessor()
	{
		min = new safety::SafeFloatArray();
		max = new safety::SafeFloatArray();
	}

    void BufferViewAccessor::deallocate()
    {
        delete min;
        delete max;
    }

    bool BufferViewAccessor::isSparse()
    {
        return sparse != nullptr;
    }

    void BufferViewAccessor::makeSparse(BufferViewAccessorSparse* sparseAccessor)
    {
        sparse = sparseAccessor;
    }

    void BufferViewAccessor::makeNotSparse()
    {
        delete sparse;
        sparse = nullptr;
    }

    BufferViewAccessor::Type BufferViewAccessor::mapTypeNameToType(std::string typeName)
    {
        if (typeName == "SCALAR")
        {
            return Type::SCALAR;
        }
        else if (typeName == "VEC2")
        {
            return Type::VEC2;
        }
        else if (typeName == "VEC3")
        {
            return Type::VEC3;
        }
        else if (typeName == "VEC4")
        {
            return Type::VEC4;
        }
        else if (typeName == "MAT2")
        {
            return Type::MAT2;
        }
        else if (typeName == "MAT3")
        {
            return Type::MAT3;
        }
        else if (typeName == "MAT4")
        {
            return Type::MAT4;
        }

        return Type::INVALID;
    }
}