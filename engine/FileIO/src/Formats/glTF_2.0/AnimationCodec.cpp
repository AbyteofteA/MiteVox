#include "AnimationCodec.h"

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/MiteVox/src/Animation/Animation.h"
#include "engine/MiteVox/src/Animation/AnimationChannel.h"

namespace fileio
{
	mitevox::Animation* AnimationCodec::fromGLTF(JSON* animationJSON)
	{
		mitevox::Animation* animationResult = new mitevox::Animation(mitevox::AnimationType::REPEATED);
		animationResult->_name = animationJSON->getFieldString("name");

		JSON* animationChannelsArrayJSON = animationJSON->getFieldArray("channels");
		if (animationChannelsArrayJSON != nullptr)
		{
			int64_t animationChannelsCount = animationChannelsArrayJSON->getArraySize();
			
			for (int64_t i = 0; i < animationChannelsCount; ++i)
			{
				//mitevox::AnimationChannel* animationChannel = new mitevox::AnimationChannel();
			}
		}
		JSON* animationSamplersJSON = animationJSON->getFieldArray("samplers");
		if (animationSamplersJSON != nullptr)
		{

		}

		return animationResult;
	}
}