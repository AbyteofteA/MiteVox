
#ifndef SHADEROPENGL_H
#define SHADEROPENGL_H

#include "engine/Renderer/src/RendererAPI/RendererSettings.h"

#include "dependencies/glm/glm.hpp"
#include "dependencies/glm/gtc/matrix_transform.hpp"
#include "dependencies/glm/gtc/type_ptr.hpp"

#include <filesystem>
namespace fs = std::filesystem;

namespace render
{
    class ShaderOpenGL
    {

    public:

        std::string name;

        unsigned int textureID = 0;
        unsigned int vertexID = 0;
        unsigned int fragmentID = 0;
        
    private:

        char* vertexFilename = nullptr;
        char* vertexSource = nullptr;
        int vertexCompileStatus = 0;
        char vertexInfoLog[512];

        char* fragmentFilename = nullptr;
        char* fragmentSource = nullptr;
        int fragmentCompileStatus = 0;
        char fragmentInfoLog[512];

        int programLinkStatus = 0;
        char programInfoLog[512];

    public:

        ShaderOpenGL(char* vertexSource, char* fragmentSource, std::string shaderName);
        ~ShaderOpenGL();

        void rename(char* _name);

        void loadVertexShader(char* source);
        void loadFragmentShader(char* source);
        void deleteVertexShader();
        void deleteFragmentShader();
        void deleteProgram();
        int compileAndLink();

        bool use();

        // Set methods for the uniforms.

        void setBool(const char* name, bool value);
        void setInt(const char* name, int value);
        void setFloat(const char* name, float value);
        void setVec2(const char* name, const glm::vec2& value);
        void setVec2(const char* name, float x, float y);
        void setVec3(const char* name, const glm::vec3& value);
        void setVec3(const char* name, float x, float y, float z);
        void setVec4(char* name, const glm::vec4& value);
        void setVec4(const char* name, float x, float y, float z, float w);
        void setMat2(const char* name, glm::mat2 mat);
        void setMat3(const char* name, glm::mat3 mat);
        void setMat4(const char* name, glm::mat4 mat);
    };
}

#endif