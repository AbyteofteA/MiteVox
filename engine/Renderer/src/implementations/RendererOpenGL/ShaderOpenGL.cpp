#include "ShaderOpenGL.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef NDEBUG
    #undef NDEBUG
#endif

#include <cassert>
#include <iostream>

namespace render
{
    ShaderOpenGL::ShaderOpenGL(std::string shaderName, char* vertexSource, char* fragmentSource, char* geometrySource)
    {
        name = shaderName;
        loadVertexShader(vertexSource);
        loadGeometryShader(geometrySource);
        loadFragmentShader(fragmentSource);
        compileAndLink();
    }

    ShaderOpenGL::ShaderOpenGL(std::string shaderName, char* computeSource)
    {
        name = shaderName;
        loadComputeShader(computeSource);
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

    void ShaderOpenGL::loadGeometryShader(char* source)
    {
        if (source != nullptr)
        {
            geometrySource = source;
            geometryID = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometryID, 1, &geometrySource, nullptr);
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

    void ShaderOpenGL::loadComputeShader(char* source)
    {
        if (source != nullptr)
        {
            computeSource = source;
            computeID = glCreateShader(GL_COMPUTE_SHADER);
            glShaderSource(computeID, 1, &computeSource, nullptr);
        }
    }

    void ShaderOpenGL::deleteProgram()
    {
        deleteVertexShader();
        deleteGeometryShader();
        deleteFragmentShader();
        deleteComputeShader();
        glDeleteProgram(shaderID);
    }

    int ShaderOpenGL::compileAndLink()
    {
        if (vertexID != 0 &&
            fragmentID != 0)
        {
            return compileAndLinkRegular();
        }
        else if (computeID != 0)
        {
            return compileAndLinkCompute();
        }

        programLinkStatus = 0;
        std::cout << "ERROR: " << "Nothing to compile" << std::endl;
        assert(programLinkStatus != 0);
        return 0;
    }

    bool ShaderOpenGL::use()
    {
        if (programLinkStatus)
        {
            glUseProgram(shaderID);
            return true;
        }

        return false;
    }

    // Set methods for the uniforms.

    void ShaderOpenGL::setBool(const char* name, bool value)
    {
        glUniform1i(glGetUniformLocation(shaderID, name), (int)value);
    }

    void ShaderOpenGL::setInt(const char* name, int value)
    {
        glUniform1i(glGetUniformLocation(shaderID, name), value);
    }

    void ShaderOpenGL::setFloat(const char* name, float value)
    {
        glUniform1f(glGetUniformLocation(shaderID, name), value);
    }

    void ShaderOpenGL::setVec2(const char* name, const glm::vec2& value)
    {
        glUniform2fv(glGetUniformLocation(shaderID, name), 1, &value[0]);
    }

    void ShaderOpenGL::setVec2(const char* name, float x, float y)
    {
        glUniform2f(glGetUniformLocation(shaderID, name), x, y);
    }

    void ShaderOpenGL::setVec3(const char* name, const glm::vec3& value)
    {
        glUniform3fv(glGetUniformLocation(shaderID, name), 1, &value[0]);
    }

    void ShaderOpenGL::setVec3(const char* name, float x, float y, float z)
    {
        glUniform3f(glGetUniformLocation(shaderID, name), x, y, z);
    }

    void ShaderOpenGL::setVec4(char* name, const glm::vec4& value)
    {
        glUniform4fv(glGetUniformLocation(shaderID, name), 1, &value[0]);
    }

    void ShaderOpenGL::setVec4(const char* name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(shaderID, name), x, y, z, w);
    }

    void ShaderOpenGL::setMat2(const char* name, glm::mat2 mat)
    {
        glUniformMatrix2fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void ShaderOpenGL::setMat3(const char* name, glm::mat3 mat)
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void ShaderOpenGL::setMat4(const char* name, glm::mat4 mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void ShaderOpenGL::setVector2D(const char* name, mathem::Vector2D vector)
    {
        glUniform2fv(glGetUniformLocation(shaderID, name), 1, vector.data);
    }

    void ShaderOpenGL::setVector3D(const char* name, mathem::Vector3D vector)
    {
        glUniform3fv(glGetUniformLocation(shaderID, name), 1, vector.data);
    }

    void ShaderOpenGL::setIntVector3D(const char* name, int x, int y, int z)
    {
        glUniform3i(glGetUniformLocation(shaderID, name), x, y, z);
    }

    void ShaderOpenGL::setVector4D(const char* name, mathem::Vector4D vector)
    {
        glUniform4fv(glGetUniformLocation(shaderID, name), 1, vector.data);
    }

    void ShaderOpenGL::setUnsignedVector4D(const char* name, mathem::Vector4D vector)
    {
        glUniform4ui(glGetUniformLocation(shaderID, name), (uint32_t)vector.x(), (uint32_t)vector.y(), (uint32_t)vector.z(), (uint32_t)vector.s());
    }

    void ShaderOpenGL::setMatrix2x2(const char* name, mathem::Matrix2x2 matrix)
    {
        glUniformMatrix2fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, matrix.data);
    }

    void ShaderOpenGL::setMatrix3x3(const char* name, mathem::Matrix3x3 matrix)
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, matrix.data);
    }

    void ShaderOpenGL::setMatrix4x4(const char* name, mathem::Matrix4x4 matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, matrix.data);
    }

    void ShaderOpenGL::setVector3DArray(const char* name, safety::SafeArray<mathem::Vector3D>* vectors)
    {
        glUniform3fv(glGetUniformLocation(shaderID, name), vectors->getElementsCount(), (float*)vectors->getElementsArray());
    }

    void ShaderOpenGL::setMatrix4x4Array(const char* name, safety::SafeArray<mathem::Matrix4x4>* matrices)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderID, name), matrices->getElementsCount(), GL_FALSE, (float*)matrices->getElementsArray());
    }

    void ShaderOpenGL::setMat4Array(const char* name, safety::SafeArray<glm::mat4>* matrices)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderID, name), matrices->getElementsCount(), GL_FALSE, (float*)matrices->getElementsArray());
    }

    void ShaderOpenGL::setCamera(Camera* camera)
    {
        setFloat("nearPlane", camera->nearCullPlane);
        setFloat("farPlane", camera->farCullPlane);
    }

    void ShaderOpenGL::setCameraMatrices(Camera* camera)
    {
        setMat4("viewMatrix", camera->getViewMatrix());
        setMat4("viewOrientationMatrix", camera->getViewOrientationMatrix());
        setMat4("projectionMatrix", camera->getProjectionMatrix());
    }

    void ShaderOpenGL::setCameraInverseMatrices(Camera* camera)
    {
        setMat4("inverseViewMatrix", glm::inverse(camera->getViewMatrix()));
        setMat4("inverseViewOrientationMatrix", glm::inverse(camera->getViewOrientationMatrix()));
        setMat4("inverseProjectionMatrix", glm::inverse(camera->getProjectionMatrix()));
    }

    void ShaderOpenGL::deleteVertexShader()
    {
        if (vertexID != 0)
        {
            glDetachShader(shaderID, vertexID);
            glDeleteShader(vertexID);
        }
    }

    void ShaderOpenGL::deleteGeometryShader()
    {
        if (geometryID != 0)
        {
            glDetachShader(shaderID, geometryID);
            glDeleteShader(geometryID);
        }
    }

    void ShaderOpenGL::deleteFragmentShader()
    {
        if (fragmentID != 0)
        {
            glDetachShader(shaderID, fragmentID);
            glDeleteShader(fragmentID);
        }
    }

    void ShaderOpenGL::deleteComputeShader()
    {
        if (computeID != 0)
        {
            glDetachShader(shaderID, computeID);
            glDeleteShader(computeID);
        }
    }

    int ShaderOpenGL::compileAndLinkRegular()
    {
        glCompileShader(vertexID);
        glGetShaderiv(vertexID, GL_COMPILE_STATUS, &vertexCompileStatus);
        glGetShaderInfoLog(vertexID, 512, nullptr, vertexInfoLog);
        if (vertexCompileStatus == 0)
        {
            std::cout << "ERROR: " << vertexInfoLog << std::endl;
            assert(vertexCompileStatus != 0);
        }

        if (geometrySource)
        {
            glCompileShader(geometryID);
            glGetShaderiv(geometryID, GL_COMPILE_STATUS, &geometryCompileStatus);
            glGetShaderInfoLog(geometryID, 512, nullptr, geometryInfoLog);
            if (geometryCompileStatus == 0)
            {
                std::cout << "ERROR: " << geometryInfoLog << std::endl;
                assert(geometryCompileStatus != 0);
            }
        }

        glCompileShader(fragmentID);
        glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &fragmentCompileStatus);
        glGetShaderInfoLog(fragmentID, 512, nullptr, fragmentInfoLog);
        if (fragmentCompileStatus == 0)
        {
            std::cout << "ERROR: " << fragmentInfoLog << std::endl;
            assert(fragmentCompileStatus != 0);
        }

        shaderID = glCreateProgram();
        glAttachShader(shaderID, vertexID);
        if (geometrySource)
        {
            glAttachShader(shaderID, geometryID);
        }
        glAttachShader(shaderID, fragmentID);
        glLinkProgram(shaderID);
        glGetProgramiv(shaderID, GL_LINK_STATUS, &programLinkStatus);
        glGetProgramInfoLog(shaderID, 512, nullptr, programInfoLog);
        if (programLinkStatus == 0)
        {
            std::cout << "ERROR: " << programInfoLog << std::endl;
            assert(programLinkStatus != 0);
        }

        return programLinkStatus;
    }

    int ShaderOpenGL::compileAndLinkCompute()
    {
        glCompileShader(computeID);
        glGetShaderiv(computeID, GL_COMPILE_STATUS, &computeCompileStatus);
        glGetShaderInfoLog(computeID, 512, nullptr, computeInfoLog);
        if (computeCompileStatus == 0)
        {
            std::cout << "ERROR: " << computeInfoLog << std::endl;
            assert(computeCompileStatus != 0);
        }

        shaderID = glCreateProgram();
        glAttachShader(shaderID, computeID);
        glLinkProgram(shaderID);
        glGetProgramiv(shaderID, GL_LINK_STATUS, &programLinkStatus);
        glGetProgramInfoLog(shaderID, 512, nullptr, programInfoLog);
        if (programLinkStatus == 0)
        {
            std::cout << "ERROR: " << programInfoLog << std::endl;
            assert(programLinkStatus != 0);
        }

        return programLinkStatus;
    }
}