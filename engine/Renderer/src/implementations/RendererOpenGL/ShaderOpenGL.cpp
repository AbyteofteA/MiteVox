#include "ShaderOpenGL.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace render
{
    ShaderOpenGL::ShaderOpenGL(char* vertexSource, char* fragmentSource, std::string shaderName)
    {
        name = shaderName;
        loadVertexShader(vertexSource);
        loadFragmentShader(fragmentSource);
        compileAndLink();
    }

    ShaderOpenGL::~ShaderOpenGL()
    {
        deleteVertexShader();
        deleteFragmentShader();
        deleteProgram();
    }

    void ShaderOpenGL::rename(char* _name)
    {
        name = _name;
    }

    void ShaderOpenGL::loadVertexShader(char* source)
    {
        if (source != nullptr)
        {
            vertexSource = source;
            vertexID = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexID, 1, &vertexSource, nullptr);
        }
    }

    void ShaderOpenGL::loadFragmentShader(char* source)
    {
        if (source != nullptr)
        {
            fragmentSource = source;
            fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentID, 1, &fragmentSource, nullptr);
        }
    }

    void ShaderOpenGL::deleteVertexShader()
    {
        glDetachShader(textureID, vertexID);
        glDeleteShader(vertexID);
    }

    void ShaderOpenGL::deleteFragmentShader()
    {
        glDetachShader(textureID, fragmentID);
        glDeleteShader(fragmentID);
    }

    void ShaderOpenGL::deleteProgram()
    {
        glDeleteProgram(textureID);
    }

    int ShaderOpenGL::compileAndLink()
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

    bool ShaderOpenGL::use()
    {
        if (programLinkStatus)
        {
            glUseProgram(textureID);
            return true;
        }

        return false;
    }

    // Set methods for the uniforms.

    void ShaderOpenGL::setBool(const char* name, bool value)
    {
        glUniform1i(glGetUniformLocation(textureID, name), (int)value);
    }

    void ShaderOpenGL::setInt(const char* name, int value)
    {
        glUniform1i(glGetUniformLocation(textureID, name), value);
    }

    void ShaderOpenGL::setFloat(const char* name, float value)
    {
        glUniform1f(glGetUniformLocation(textureID, name), value);
    }

    void ShaderOpenGL::setVec2(const char* name, const glm::vec2& value)
    {
        glUniform2fv(glGetUniformLocation(textureID, name), 1, &value[0]);
    }

    void ShaderOpenGL::setVec2(const char* name, float x, float y)
    {
        glUniform2f(glGetUniformLocation(textureID, name), x, y);
    }

    void ShaderOpenGL::setVec3(const char* name, const glm::vec3& value)
    {
        glUniform3fv(glGetUniformLocation(textureID, name), 1, &value[0]);
    }

    void ShaderOpenGL::setVec3(const char* name, float x, float y, float z)
    {
        glUniform3f(glGetUniformLocation(textureID, name), x, y, z);
    }

    void ShaderOpenGL::setVec4(char* name, const glm::vec4& value)
    {
        glUniform4fv(glGetUniformLocation(textureID, name), 1, &value[0]);
    }

    void ShaderOpenGL::setVec4(const char* name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(textureID, name), x, y, z, w);
    }

    void ShaderOpenGL::setMat2(const char* name, glm::mat2 mat)
    {
        glUniformMatrix2fv(glGetUniformLocation(textureID, name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void ShaderOpenGL::setMat3(const char* name, glm::mat3 mat)
    {
        glUniformMatrix3fv(glGetUniformLocation(textureID, name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void ShaderOpenGL::setMat4(const char* name, glm::mat4 mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(textureID, name), 1, GL_FALSE, glm::value_ptr(mat));
    }
}