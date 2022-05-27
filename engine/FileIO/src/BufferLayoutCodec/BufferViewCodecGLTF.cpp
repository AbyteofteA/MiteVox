#include "BufferViewCodec.h"

namespace fileio
{
	void BufferViewCodec::fromGLTF(
		mitevox::BufferView* bufferViewResult, 
		JSON* bufferViewJSON,
		safety::SafeArray<safety::SafeByteArray*>* buffers)
	{
		bufferViewResult->name = bufferViewJSON->getFieldString("name");
		bufferViewResult->byteOffset = (uint64_t)bufferViewJSON->getFieldNumber("byteOffset");
		bufferViewResult->byteLength = (uint64_t)bufferViewJSON->getFieldNumber("byteLength");
		bufferViewResult->byteStride = (uint16_t)bufferViewJSON->getFieldNumber("byteStride");

		JSON* numberJSON = bufferViewJSON->getField("buffer");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t bufferIndex = (int32_t)numberJSON->getNumber();
				bufferViewResult->buffer = buffers->getElement(bufferIndex);
			}
		}

		JSON* targetJSON = bufferViewJSON->getField("target");
		if (targetJSON != nullptr)
		{
			if (targetJSON->isNumber())
			{
				bufferViewResult->target = (mitevox::BufferView::TargetBufferType)targetJSON->getNumber();
			}
		}
	}
}