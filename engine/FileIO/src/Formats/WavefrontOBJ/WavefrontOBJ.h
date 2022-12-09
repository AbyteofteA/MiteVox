#ifndef WAVEFRONTOBJ_H
#define WAVEFRONTOBJ_H

#include "engine/FileIO/src/SerializationStatus.h"
#include "engine/Math/src/Vector.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <cstdint>
#include <string>

namespace fileio
{
    struct WavefrontOBJPolygon
    {
        safety::SafeArray<int32_t> positions;
        safety::SafeArray<int32_t> textureCoords;
        safety::SafeArray<int32_t> normals;
        size_t verticesCount = 0;
    };

    class WavefrontOBJ
    {
    public:

        WavefrontOBJ();
        ~WavefrontOBJ();

        SerializationStatus serialize(safety::SafeByteArray* fileData);
        DeserializationStatus deserialize(safety::SafeByteArray* fileData);

    private:

        safety::SafeArray<mathem::Vector3D> positions;
        safety::SafeArray<mathem::Vector3D> normals;
        safety::SafeArray<mathem::Vector3D> textureCoords;
        safety::SafeArray<WavefrontOBJPolygon> polygons;

        unsigned int vertexBufferID = -1;
        mathem::Vector3D minPosition, maxPosition;
    };
}

#endif