#ifndef BUFFERVIEWCODEC_H
#define BUFFERVIEWCODEC_H

#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace fileio
{
	class BufferViewCodec
	{
	public:

		static mitevox::BufferView* fromGLTF(
			JSON* bufferViewJSON, 
			safety::SafeArray<safety::SafeByteArray*>* buffers);
	};
}

#endif