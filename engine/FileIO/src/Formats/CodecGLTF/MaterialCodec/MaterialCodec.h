
#ifndef MATERIAL_CODEC_H
#define MATERIAL_CODEC_H

#include "engine/MiteVox/src/Material/Material.h"
#include "engine/MiteVox/src/Material/Texture.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace fileio
{
	class MaterialCodec
	{
	public:

		static void fromGLTF(
			mitevox::Material* materialResult,
			JSON* materialJSON,
			safety::SafeArray<mitevox::Texture>* textures);
		static void toGLTF(
			JSON* materialJSONResult,
			mitevox::Material* material,
			safety::SafeArray<mitevox::Texture>* textures);
	};
}

#endif