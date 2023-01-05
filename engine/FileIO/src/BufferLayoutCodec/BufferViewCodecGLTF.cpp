#include "BufferViewCodec.h"

namespace fileio
{
	void BufferViewCodec::fromGLTF(
		mitevox::BufferView* bufferViewResult, 
		JSON* bufferViewJSON,
		safety::SafeArray<safety::SafeByteArray*>* buffers)
	{
		bufferViewResult->name = bufferViewJSON->getFieldStringOrDefault("name", "Untitled");
		bufferViewResult->byteOffset = (uint64_t)bufferViewJSON->getFieldNumberOrDefault("byteOffset", 0.0);
		bufferViewResult->byteLength = (uint64_t)bufferViewJSON->getFieldNumberOrDefault("byteLength", 0.0);
		bufferViewResult->byteStride = (uint16_t)bufferViewJSON->getFieldNumberOrDefault("byteStride", 0.0);

		if (JSON* numberJSON = bufferViewJSON->getField("buffer"))
		{
			int32_t bufferIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0);
			bufferViewResult->buffer = buffers->getElement(bufferIndex);
		}

		if (JSON* targetJSON = bufferViewJSON->getField("target"))
		{
			bufferViewResult->target = (mitevox::BufferView::TargetBufferType)targetJSON->getNumberOrDefault(
				(double)mitevox::BufferView::TargetBufferType::ARRAY_BUFFER);
		}
	}
}