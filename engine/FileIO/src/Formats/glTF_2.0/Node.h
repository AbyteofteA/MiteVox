#ifndef GLTFNODE_H
#define GLTFNODE_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"
//#include "engine/Math/src/LinearAlgebra/Transform.h"

#include <string>
#include <cstdint>

namespace fileio
{
    class Node
    {
    public:

        render::Camera* camera = nullptr;
        int32_t skinIndex;
        mitevox::Mesh* mesh;
        std::string name;
        //mathem::Transform transform;
        safety::SafeFloatArray weights;
        safety::SafeFloatArray matrix;
        safety::SafeArray<Node*> children;

        Node();
        void fromGLTF(
            JSON* nodeJSON, 
            safety::SafeArray<render::Camera*>* cameras,
            safety::SafeArray<mitevox::Mesh*>* meshes,
            safety::SafeArray<Node*>* nodes);
    };
}

#endif