#include "Animation.h"

#include <cmath>

namespace mitevox
{
	Animation::Animation(AnimationType animationType)
	{
		_isPaused = true;
		_currentTime = 0.0f;
		_period = 0.0f;
		_animationType = animationType;
	}

	void Animation::setType(AnimationType animationType)
	{
		_animationType = animationType;
	}

	void Animation::start()
	{
		_isPaused = false;
		_currentTime = 0.0f;
	}

	void Animation::pause()
	{
		_isPaused = true;
	}

	void Animation::resume()
	{
		_isPaused = false;
	}

	float Animation::getCurrentTime()
	{
		return _currentTime;
	}

	void Animation::update(float deltaTime)
	{
		if (_isPaused)
		{
			return;
		}

		_currentTime += deltaTime;

		switch (_animationType)
		{
		case AnimationType::SINGLE_SHOT:
			if (_currentTime > _period)
			{
				_isPaused = true;
				return;
			}
			break;

		case AnimationType::REPEATED:
			if (_currentTime > _period)
			{
				_currentTime = fmod(_currentTime, _period);
			}
			break;

		default:
			return;
		}

		int64_t channelsCount = _channels.getElementsCount();
		for (int64_t i = 0; i < channelsCount; ++i)
		{
			_channels.getElement(i)->apply(_currentTime);
		}
	}

	void Animation::addChannel(AnimationChannelBase* animationChannel)
	{
		float animationChannelPeriod = animationChannel->getPeriod();
		if (_period < animationChannelPeriod)
		{
			_period = animationChannelPeriod;
		}
		_channels.appendElement(animationChannel);
	}

	void Animation::removeAllChannels()
	{
		_period = 0.0f;
		_channels.deallocate();
	}
}