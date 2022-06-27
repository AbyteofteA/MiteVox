#ifndef MESHATTRIBUTESET_H
#define MESHATTRIBUTESET_H

#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"

namespace mitevox
{
    class MeshAttributeSet
    {
    public:

        BufferViewAccessor* positionAccessor = nullptr;
        BufferViewAccessor* normalAccessor = nullptr;
        BufferViewAccessor* tangentAccessor = nullptr;
        BufferViewAccessor* textureCoordAccessor_0 = nullptr;
        BufferViewAccessor* textureCoordAccessor_1 = nullptr;
        BufferViewAccessor* colorAccessor_0 = nullptr;
        BufferViewAccessor* jointsAccessor_0 = nullptr;
        BufferViewAccessor* weightsAccessor_0 = nullptr;

        void makeSeparateCopyTo(MeshAttributeSet* resultMeshAttributeSet);
    };
}

#endif
