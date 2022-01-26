#ifndef WAVEFRONTOBJREGEXPACK_H
#define WAVEFRONTOBJREGEXPACK_H

#include <regex>

namespace fileio
{
    class WavefrontOBJRegexPack
    {
    public:

        std::regex commentLines;

        std::regex geometricVertex;
        std::regex textureVertex;
        std::regex vertexNormal;

        std::regex point;
        std::regex line;
        std::regex polygonSimple;
        std::regex polygonWithTexture;
        std::regex polygonWithNormals;
        std::regex polygonComplete;

        std::regex group;
        std::regex smoothingGroup;
        std::regex object;

        std::regex useMaterial;
        std::regex materialLibraryFile;

        // Helpers

        std::regex vertexWithTexture;
        std::regex vertexWithNormal;
        std::regex vertexComplete;

        std::regex intNumber;
        std::regex floatNumber;

        WavefrontOBJRegexPack();
    };
}

#endif