#ifndef ANIMATIONCHANNELBASE_H
#define ANIMATIONCHANNELBASE_H

namespace mitevox
{
    class AnimationChannelBase
    {
    public:

        virtual void apply(float time) = 0;
        virtual float getPeriod() = 0;
    };
}

#endif