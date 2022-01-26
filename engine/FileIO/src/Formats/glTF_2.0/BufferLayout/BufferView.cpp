#include "BufferView.h"

namespace fileio
{
	BufferView::BufferView() :
		buffer(nullptr),
		byteOffset(0),
		byteLength(0),
		byteStride(0),
		target(TargetBufferType::ARRAY_BUFFER),
		ID(0)
	{

	}

	void BufferView::fromGLTF(JSON* bufferViewJSON, safety::SafeArray<safety::SafeByteArray*>* buffers)
	{
		name = bufferViewJSON->getFieldString("name");
		byteOffset = (uint64_t)bufferViewJSON->getFieldNumber("byteOffset");
		byteLength = (uint64_t)bufferViewJSON->getFieldNumber("byteLength");
		byteStride = (uint16_t)bufferViewJSON->getFieldNumber("byteStride");

		JSON* numberJSON = bufferViewJSON->getField("buffer");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t bufferIndex = (int32_t)numberJSON->getNumber();
				buffer = buffers->getElement(bufferIndex);
			}
		}

		JSON* targetJSON = bufferViewJSON->getField("target");
		if (targetJSON != nullptr)
		{
			if (targetJSON->isNumber())
			{
				target = (BufferView::TargetBufferType)targetJSON->getNumber();
			}
		}
	}
}