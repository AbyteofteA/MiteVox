#ifndef BUFFERVIEWACCESSORCODEC_H
#define BUFFERVIEWACCESSORCODEC_H

#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace fileio
{
	class BufferViewAccessorCodec
	{
	public:

		static void fromGLTF(
			mitevox::BufferViewAccessor* bufferViewAccessorResult,
			JSON* accessorJSON,
			safety::SafeArray<mitevox::BufferView*>* bufferViews);
		static void toGLTF(
			JSON* accessorJSONResult,
			mitevox::BufferViewAccessor* bufferViewAccessor,
			safety::SafeArray<mitevox::BufferView*>* bufferViews);

	private:

		static void collectMinMaxFromGLTF(mitevox::BufferViewAccessor* bufferViewAccessor, JSON* accessorJSON);
		static void sparseFromGLTF(mitevox::BufferViewAccessor* bufferViewAccessor, JSON* accessorSparseJSON);

		static void saveMinMaxToGLTF(JSON* accessorJSON, mitevox::BufferViewAccessor* bufferViewAccessor);
		static void sparseToGLTF(JSON* accessorSparseJSON, mitevox::BufferViewAccessor* bufferViewAccessor);
	};
}

#endif