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

	void BufferViewCodec::toGLTF(
		JSON* bufferViewJSONResult,
		mitevox::BufferView* bufferView,
		safety::SafeArray<safety::SafeByteArray*>* buffers)
	{
		bufferViewJSONResult->setField("name", bufferView->name);
		bufferViewJSONResult->setField("byteOffset", (double)bufferView->byteOffset);
		bufferViewJSONResult->setField("byteLength", (double)bufferView->byteLength);
		bufferViewJSONResult->setField("target", (double)bufferView->target);
		if (bufferView->byteStride > 0)
		{
			bufferViewJSONResult->setField("byteStride", (double)bufferView->byteStride);
		}

		size_t buffersCount = buffers->getElementsCount();
		for (size_t i = 0; i < buffersCount; ++i)
		{
			if (bufferView->buffer == buffers->getElement(i))
			{
				bufferViewJSONResult->setField("buffer", (double)i);
				break;
			}
		}
	}
}