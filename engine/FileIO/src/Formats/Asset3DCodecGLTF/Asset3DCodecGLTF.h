#ifndef CODECGLTF_H
#define CODECGLTF_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/MiteVox/src/Playground/Asset3D.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/FileIO/src/SerializationStatus.h"
#include "engine/FileIO/src/Formats/FileFormatBase.h"

namespace fileio
{
    DeserializationStatus fromGLTF(std::string filePath, mitevox::Asset3D* asset3DResult); // TODO: return correct status
    SerializationStatus toGLTF(std::string filePath, mitevox::Asset3D* asset3D); // TODO: return correct status

    DeserializationStatus fromGLTF(std::string filePath, mitevox::Asset3D* asset3DResult, JSON* asset3DJSON); // TODO: return correct status
    SerializationStatus toGLTF(std::string filePath, mitevox::Asset3D* asset3D, JSON* asset3DJSONResult); // TODO: return correct status
}

#endif

