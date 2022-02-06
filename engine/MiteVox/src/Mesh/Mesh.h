#ifndef GLTFMESH_H
#define GLTFMESH_H

#include "engine/MiteVox/src/Mesh/MeshPrimitive.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace mitevox
{
    class Mesh
    {
    public:

        safety::SafeArray<MeshPrimitive*> primitives;
        safety::SafeFloatArray weights;
        std::string name;
    };
}

#endif