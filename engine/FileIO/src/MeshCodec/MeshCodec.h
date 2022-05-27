#ifndef MESHCODEC_H
#define MESHCODEC_H

#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/MiteVox/src/Mesh/MeshPrimitive.h"
#include "engine/MiteVox/src/Material/Material.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace fileio
{
	class MeshCodec
	{
	public:

		static void fromGLTF(
			mitevox::Mesh* meshResult,
			JSON* meshJSON,
			safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
			safety::SafeArray<mitevox::Material*>* materials);

	private:

		static void meshPrimitiveFromGLTF(
			mitevox::MeshPrimitive* meshPrimitiveResult,
			JSON* meshPrimitiveJSON,
			safety::SafeArray<mitevox::BufferViewAccessor*>* accessors,
			safety::SafeArray<mitevox::Material*>* materials);

		static void collectAttributesFromJSON(
			mitevox::MeshAttributeSet* meshAttributeSet,
			JSON* meshAttributesJSON, 
			safety::SafeArray<mitevox::BufferViewAccessor*>* accessors);
	};
}

#endif