#include "WavefrontOBJRegexPack.h"

#include "engine/FileIO/src/Formats/WavefrontOBJ/WavefrontOBJRegex.h"

#include <regex>

namespace fileio
{
    WavefrontOBJRegexPack::WavefrontOBJRegexPack()
    {
        commentLines = std::regex(OBJ_REGEX_COMMENT_LINES);

        geometricVertex = std::regex(OBJ_REGEX_GEOMETRIC_VERTEX);
        textureVertex = std::regex(OBJ_REGEX_TEXTURE_VERTEX);
        vertexNormal = std::regex(OBJ_REGEX_VERTEX_NORMAL);

        point = std::regex(OBJ_REGEX_POINT);
        line = std::regex(OBJ_REGEX_LINE);
        polygonSimple = std::regex(OBJ_REGEX_FACE);
        polygonWithTexture = std::regex(OBJ_REGEX_FACE_WITH_TEXTURE);
        polygonWithNormals = std::regex(OBJ_REGEX_FACE_WITH_NORMALS);
        polygonComplete = std::regex(OBJ_REGEX_FACE_COMPLETE);
        
        group = std::regex(OBJ_REGEX_GROUP);
        smoothingGroup = std::regex(OBJ_REGEX_SMOOTHING_GROUP);
        object = std::regex(OBJ_REGEX_OBJECT);

        useMaterial = std::regex(OBJ_REGEX_USE_MATERIAL);
        materialLibraryFile = std::regex(OBJ_REGEX_MATERIAL_LIBRARY_FILE);

        // Helpers

        vertexWithTexture = std::regex(OBJ_REGEX_VERTEX_WITH_TEXTURE);
        vertexWithNormal = std::regex(OBJ_REGEX_VERTEX_WITH_NORMALS);
        vertexComplete = std::regex(OBJ_REGEX_VERTEX_COMPLETE);

        intNumber = std::regex(OBJ_REGEX_INT_NUMBER);
        floatNumber = std::regex(OBJ_REGEX_FLOAT_NUMBER);
    }
}