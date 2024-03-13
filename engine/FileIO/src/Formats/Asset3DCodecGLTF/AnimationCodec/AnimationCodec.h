#ifndef ANIMATIONCODEC_H
#define ANIMATIONCODEC_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/MiteVox/src/Animation/Animation.h"
#include "engine/MiteVox/src/Playground/Node.h"

namespace fileio
{
	class AnimationCodec
	{
	public:

		static void fromGLTF(
			mitevox::Animation* animationResult, 
			JSON* animationJSON, 
			safety::SafeArray<mitevox::Node*>* nodes,
			safety::SafeArray<mitevox::BufferViewAccessor*>* accessors);
	};
}

#endif