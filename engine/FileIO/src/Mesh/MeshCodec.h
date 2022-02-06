#ifndef MeshCodec_H
#define GLTF_H

#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/MiteVox/src/Mesh/MeshPrimitive.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace fileio
{
	class MeshCodec
	{
	public:

		static mitevox::Mesh* fromGLTF(
			JSON* meshJSON,
			safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
			safety::SafeArray<Material*>* materials);

	private:

		static mitevox::MeshPrimitive* meshPrimitiveFromGLTF(
			JSON* meshPrimitiveJSON,
			safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
			safety::SafeArray<Material*>* materials);

		static void collectAttributesFromJSON(
			JSON* meshAttributesJSON, 
			mitevox::MeshAttributeSet* meshAttributeSet,
			safety::SafeArray<mitevox::BufferViewAccessor*>* accessors);
	};
}

#endif