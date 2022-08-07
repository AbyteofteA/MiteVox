#ifndef MESHATTRIBUTESET_H
#define MESHATTRIBUTESET_H

#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"

namespace mitevox
{
    class MeshAttributeSet
    {
    public:

        static const size_t attributesCount = 8;

        union 
        {
            struct
            {
                BufferViewAccessor* positionAccessor = nullptr;
                BufferViewAccessor* normalAccessor = nullptr;
                BufferViewAccessor* textureCoordAccessor_0 = nullptr;
                BufferViewAccessor* textureCoordAccessor_1 = nullptr;
                BufferViewAccessor* colorAccessor_0 = nullptr;
                BufferViewAccessor* jointsAccessor_0 = nullptr;
                BufferViewAccessor* weightsAccessor_0 = nullptr;
                BufferViewAccessor* tangentAccessor = nullptr;
            } byName;
            BufferViewAccessor* byIndex[attributesCount];
        };

        MeshAttributeSet();
        
        void makeSeparateCopyTo(MeshAttributeSet* resultMeshAttributeSet);
    };
}

#endif
