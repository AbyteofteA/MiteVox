#ifndef GLTFMESH_H
#define GLTFMESH_H

#include "engine/FileIO/src/Formats/glTF_2.0/Mesh/MeshPrimitive.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace fileio
{
    class Mesh
    {
    public:

        safety::SafeArray<MeshPrimitive*> primitives;
        safety::SafeFloatArray weights;
        std::string name;

        Mesh();
        void fromGLTF(
            JSON* meshJSON,
            safety::SafeArray<BufferViewAccessor*>* accessors,
            safety::SafeArray<Material*>* materials);
    };
}

#endif