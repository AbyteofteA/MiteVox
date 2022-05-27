#ifndef ANIMATIONCODEC_H
#define ANIMATIONCODEC_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/MiteVox/src/Animation/Animation.h"

namespace fileio
{
	class AnimationCodec
	{
	public:

		static mitevox::Animation* fromGLTF(JSON* animationJSON);
	};
}

#endif