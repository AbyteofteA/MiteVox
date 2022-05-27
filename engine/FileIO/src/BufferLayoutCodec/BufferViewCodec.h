#ifndef BUFFERVIEWCODEC_H
#define BUFFERVIEWCODEC_H

#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace fileio
{
	class BufferViewCodec
	{
	public:

		static void fromGLTF(
			mitevox::BufferView* bufferViewResult,
			JSON* bufferViewJSON,
			safety::SafeArray<safety::SafeByteArray*>* buffers);
	};
}

#endif