#include "BufferView.h"

namespace mitevox
{
	void* BufferView::getDataStart()
	{
		return buffer->getElementsArray() + byteOffset;
	}

	void BufferView::makeSeparateCopyTo(BufferView* resultBufferView)
	{
		*resultBufferView = *this;
		resultBufferView->buffer = this->buffer->getPart(this->byteOffset, this->byteLength);
		resultBufferView->byteOffset = 0;
		resultBufferView->ID = 0;
	}
}