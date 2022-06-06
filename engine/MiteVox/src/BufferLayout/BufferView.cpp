#include "BufferView.h"

namespace mitevox
{
	void* BufferView::getDataStart()
	{
		return buffer->getElementsArray() + byteOffset;
	}
}