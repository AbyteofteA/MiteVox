#ifndef ANIMATIONCHANNEL_H
#define ANIMATIONCHANNEL_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/MiteVox/src/Animation/AnimationChannelBase.h"

namespace mitevox
{
    enum struct InterpolationType
    {
        STEP,
        LINEAR,
        CUBIC_SPLINE
    };

    template <typename T>
    class AnimationFrame
    {
    public:
        float time;
        T data;
    };

    template <typename T>
    class AnimationChannel : public AnimationChannelBase
    {
    public:

        inline AnimationChannel(T* target, InterpolationType interpolationType, size_t framesCount);
        inline void setFrame(int64_t frameIndex, float time, T data);

        // AnimationChannelBase
        inline void apply(float time);
        inline float getPeriod();

    private:

        T* _target;
        InterpolationType _interpolationType;
        safety::SafeArray<AnimationFrame<T>> _frames;
    };
    

    // IMPLEMENTATION BELOW //


    template <typename T>
    inline AnimationChannel<T>::AnimationChannel(T* target, InterpolationType interpolationType, size_t framesCount)
    {
        _target = target;
        _interpolationType = interpolationType;
        _frames.resize(framesCount);
    }

    template <typename T>
    inline void AnimationChannel<T>::setFrame(int64_t frameIndex, float time, T data)
    {
        AnimationFrame<T> frame = { time, data };
        _frames.setElement(frameIndex, frame);
    }

    /// <summary>
    /// NOTE: T must define multiplication and division by a number, 
    /// addition and subtruction with a T.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="time"></param>
    template <typename T>
    inline void AnimationChannel<T>::apply(float time)
    {
        int64_t currentFrameIndex = -1; // -1 means no animation frame, no changes to the target
        int64_t frameCount = _frames.getElementsCount();
        for (int64_t i = 0; i < frameCount; ++i)
        {
            if (time < _frames.getElement(i).time)
            {
                currentFrameIndex = i - 1;
                break;
            }
        }

        if (currentFrameIndex < 0)
        {
            *_target = _frames.getElement(0).data;
            return;
        }

        switch (_interpolationType)
        {
        case InterpolationType::STEP:
            *_target = _frames.getElement(currentFrameIndex).data;
            break;

        case InterpolationType::LINEAR:
        {
            AnimationFrame<T> currentFrame = _frames.getElement(currentFrameIndex);
            AnimationFrame<T> nextFrame = _frames.getElement(currentFrameIndex + 1);
            T deltaFrameData = nextFrame.data - currentFrame.data;
            float slope = ((time - currentFrame.time) / (nextFrame.time - currentFrame.time));

            // Interpolated data calculation
            deltaFrameData *= slope;
            deltaFrameData += currentFrame.data;
            *_target = deltaFrameData;
            break;
        }

        case InterpolationType::CUBIC_SPLINE:
            // TODO: Implement InterpolationType::CUBIC_SPLINE
            *_target = _frames.getElement(currentFrameIndex).data;
            break;
        }
    }

    template <typename T>
    inline float AnimationChannel<T>::getPeriod()
    {
        int64_t framesCount = _frames.getElementsCount();
        AnimationFrame<T> lastFrame = _frames.getElement(framesCount - 1);
        return lastFrame.time;
    }
}

#endif
