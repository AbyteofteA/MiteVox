#include "BufferViewCodec.h"

namespace fileio
{
	mitevox::BufferView* BufferViewCodec::fromGLTF(
		JSON* bufferViewJSON,
		safety::SafeArray<safety::SafeByteArray*>* buffers)
	{
		mitevox::BufferView* bufferView = new mitevox::BufferView();
		bufferView->name = bufferViewJSON->getFieldString("name");
		bufferView->byteOffset = (uint64_t)bufferViewJSON->getFieldNumber("byteOffset");
		bufferView->byteLength = (uint64_t)bufferViewJSON->getFieldNumber("byteLength");
		bufferView->byteStride = (uint16_t)bufferViewJSON->getFieldNumber("byteStride");

		JSON* numberJSON = bufferViewJSON->getField("buffer");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				int32_t bufferIndex = (int32_t)numberJSON->getNumber();
				bufferView->buffer = buffers->getElement(bufferIndex);
			}
		}

		JSON* targetJSON = bufferViewJSON->getField("target");
		if (targetJSON != nullptr)
		{
			if (targetJSON->isNumber())
			{
				bufferView->target = (mitevox::BufferView::TargetBufferType)targetJSON->getNumber();
			}
		}

		return bufferView;
	}
}