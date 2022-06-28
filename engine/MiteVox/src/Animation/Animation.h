#ifndef ANIMATION_H
#define ANIMATION_H

#include "engine/MiteVox/src/Animation/AnimationChannelBase.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace fileio
{
	class AnimationCodec;
}

namespace mitevox
{
	enum class AnimationType
	{
		SINGLE_SHOT,
		REPEATED
	};

	class Animation
	{
		friend class fileio::AnimationCodec;

	public:

		Animation(AnimationType animationType = AnimationType::REPEATED);
		void setType(AnimationType animationType);
		void start();
		void pause();
		void resume();
		bool isActive();
		float getCurrentTime();
		void update(float deltaTime);
		void addChannel(AnimationChannelBase* animationChannel);
		void removeAllChannels();

	protected:

		std::string _name;
		bool _isPaused;
		AnimationType _animationType;
		float _currentTime;
		float _period;
		safety::SafeArray<AnimationChannelBase*> _channels;
	};
}

#endif