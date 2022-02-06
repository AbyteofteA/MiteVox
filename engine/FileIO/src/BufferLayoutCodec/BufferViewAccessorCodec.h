#ifndef BUFFERVIEWACCESSORCODEC_H
#define BUFFERVIEWACCESSORCODEC_H

#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace fileio
{
	class BufferViewAccessorCodec
	{
	public:

		static mitevox::BufferViewAccessor* fromGLTF(
			JSON* accessorJSON, 
			safety::SafeArray<mitevox::BufferView*>* bufferViews);

	private:

		static void collectMinMaxFromGLTF(JSON* accessorJSON, mitevox::BufferViewAccessor* bufferViewAccessor);
		static void sparseFromGLTF(JSON* accessorSparseJSON, mitevox::BufferViewAccessor* bufferViewAccessor);
	};
}

#endif