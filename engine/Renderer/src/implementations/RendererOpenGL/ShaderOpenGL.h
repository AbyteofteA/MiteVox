
#ifndef SHADEROPENGL_H
#define SHADEROPENGL_H

#include "FileIO/src/FileIO_Headers.h"
#include "Renderer/src/RendererAPI/Renderer_Settings.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <filesystem>
namespace fs = std::filesystem;


namespace render
{
    extern RendererSettings RendererSettingsDebug;

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

        ShaderOpenGL(std::string _name)
        {
            name = _name;
        }
        ~ShaderOpenGL()
        {
            deleteVertexShader();
            deleteFragmentShader();
            deleteProgram();
        }

        void rename(char* _name)
        {
            name = _name;
        }

        void loadVertexShader(char* source)
        {
            if (source != nullptr)
            {
                vertexSource = source;
                vertexID = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertexID, 1, &vertexSource, nullptr);
            }
        }

        void loadFragmentShader(char* source)
        {
            if (source != nullptr)
            {
                fragmentSource = source;
                fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragmentID, 1, &fragmentSource, nullptr);
            }
        }

        void deleteVertexShader()
        {
            glDetachShader(textureID, vertexID);
            glDeleteShader(vertexID);
        }
        void deleteFragmentShader()
        {
            glDetachShader(textureID, fragmentID);
            glDeleteShader(fragmentID);
        }
        void deleteProgram()
        {
            glDeleteProgram(textureID);
        }

        int compileAndLink()
        {
            glCompileShader(vertexID);
            glGetShaderiv(vertexID, GL_COMPILE_STATUS, &vertexCompileStatus);
            glGetShaderInfoLog(vertexID, 512, nullptr, vertexInfoLog);

            glCompileShader(fragmentID);
            glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &fragmentCompileStatus);
            glGetShaderInfoLog(fragmentID, 512, nullptr, fragmentInfoLog);

            if (vertexCompileStatus != 0 && fragmentCompileStatus != 0)
            {
                textureID = glCreateProgram();
                glAttachShader(textureID, vertexID);
                glAttachShader(textureID, fragmentID);
                glLinkProgram(textureID);
                glGetProgramiv(textureID, GL_LINK_STATUS, &programLinkStatus);
                glGetProgramInfoLog(textureID, 512, nullptr, programInfoLog);
            }
            return programLinkStatus;
        }

        bool use()
        {
            if (programLinkStatus)
            {
                glUseProgram(textureID);
                return true;
            }

            return false;
        }

        // Set methods for the uniforms.

        void setBool(const char* name, bool value)
        {
            glUniform1i(glGetUniformLocation(textureID, name), (int)value);
        }

        void setInt(const char* name, int value)
        {
            glUniform1i(glGetUniformLocation(textureID, name), value);
        }

        void setFloat(const char* name, float value)
        {
            glUniform1f(glGetUniformLocation(textureID, name), value);
        }

        void setVec2(const char* name, const glm::vec2& value)
        {
            glUniform2fv(glGetUniformLocation(textureID, name), 1, &value[0]);
        }
        void setVec2(const char* name, float x, float y)
        {
            glUniform2f(glGetUniformLocation(textureID, name), x, y);
        }

        void setVec3(const char* name, const glm::vec3& value)
        {
            glUniform3fv(glGetUniformLocation(textureID, name), 1, &value[0]);
        }
        void setVec3(const char* name, float x, float y, float z)
        {
            glUniform3f(glGetUniformLocation(textureID, name), x, y, z);
        }

        void setVec4(char* name, const glm::vec4& value)
        {
            glUniform4fv(glGetUniformLocation(textureID, name), 1, &value[0]);
        }
        void setVec4(const char* name, float x, float y, float z, float w)
        {
            glUniform4f(glGetUniformLocation(textureID, name), x, y, z, w);
        }

        void setMat2(const char* name, glm::mat2 mat)
        {
            glUniformMatrix2fv(glGetUniformLocation(textureID, name), 1, GL_FALSE, glm::value_ptr(mat));
        }

        void setMat3(const char* name, glm::mat3 mat)
        {
            glUniformMatrix3fv(glGetUniformLocation(textureID, name), 1, GL_FALSE, glm::value_ptr(mat));
        }

        void setMat4(const char* name, glm::mat4 mat)
        {
            glUniformMatrix4fv(glGetUniformLocation(textureID, name), 1, GL_FALSE, glm::value_ptr(mat));
        }
    };

    inline ShaderOpenGL* compileShader(char* vertexSource, char* fragmentSource, std::string shaderName)
    {
        ShaderOpenGL* shader = new ShaderOpenGL(shaderName);
        shader->loadVertexShader(vertexSource);
        shader->loadFragmentShader(fragmentSource);
        shader->compileAndLink();

        return shader;
    }

    inline void loadShaders(std::string dirName, std::vector<ShaderOpenGL*>* shaders)
    {
        /*fs::path currentPath = fs::path(dirName);

        for (fs::directory_entry de : fs::directory_iterator(currentPath))
        {
            for (fs::directory_entry de : fs::directory_iterator(currentPath))
            {
            std::string filename = de.path().filename().replace_extension("").string();
            std::string path = de.path().string();

            char* vertexSource = nullptr;
            char* fragmentSource = nullptr;
        }*/


        char* vertexSourceB = nullptr;
        char* fragmentSourceB = nullptr;
        char* vertexSourceS = nullptr;
        char* fragmentSourceS = nullptr;
        char* vertexSourceP = nullptr;
        char* fragmentSourceP = nullptr;

        std::string path1;
        std::string path2;
        std::string path3;
        std::string path4;
        std::string path5;
        std::string path6;

        if (RendererSettingsDebug.debug)
        {
            path1 = "../Renderer/shaders/basic/basicShader.vert";
            path2 = "../Renderer/shaders/basic/basicShader.frag";
            path3 = "../Renderer/shaders/skybox/skyboxShader.vert";
            path4 = "../Renderer/shaders/skybox/skyboxShader.frag";
            path5 = "../Renderer/shaders/primitive/primitiveShader.vert";
            path6 = "../Renderer/shaders/primitive/primitiveShader.frag";
        }
        else
        {
            path1 = "../../../engine/Renderer/shaders/basic/basicShader.vert";
            path2 = "../../../engine/Renderer/shaders/basic/basicShader.frag";
            path3 = "../../../engine/Renderer/shaders/skybox/skyboxShader.vert";
            path4 = "../../../engine/Renderer/shaders/skybox/skyboxShader.frag";
            path5 = "../../../engine/Renderer/shaders/primitive/primitiveShader.vert";
            path6 = "../../../engine/Renderer/shaders/primitive/primitiveShader.frag";
        }

        fileio::fileLoader.loadAndParseAsync(path1, (void**)&vertexSourceB);
        fileio::fileLoader.loadAndParseAsync(path2, (void**)&fragmentSourceB);
        fileio::fileLoader.loadAndParseAsync(path3, (void**)&vertexSourceS);
        fileio::fileLoader.loadAndParseAsync(path4, (void**)&fragmentSourceS);
        fileio::fileLoader.loadAndParseAsync(path5, (void**)&vertexSourceP);
        fileio::fileLoader.loadAndParseAsync(path6, (void**)&fragmentSourceP);

        fileio::fileLoader.awaitAll();

        // Setting up shaders
        ShaderOpenGL* basicShader = compileShader(vertexSourceB, fragmentSourceB, "Basic Shader");
        shaders->push_back(basicShader);

        ShaderOpenGL* skyboxShader = compileShader(vertexSourceS, fragmentSourceS, "Skybox Shader");
        shaders->push_back(skyboxShader);

        ShaderOpenGL* primitiveShader = compileShader(vertexSourceP, fragmentSourceP, "Primitive Shader");
        shaders->push_back(primitiveShader);
    }

}

#endif