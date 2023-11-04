#ifndef SHADEROPENGL_H
#define SHADEROPENGL_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Matrix.h"
#include "engine/CodeSafety/src/SafeArray.h"

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

        unsigned int shaderID = 0;
        unsigned int vertexID = 0;
        unsigned int geometryID = 0;
        unsigned int fragmentID = 0;
        unsigned int computeID = 0;
        
        ShaderOpenGL(std::string shaderName, char* vertexSource, char* fragmentSource, char* geometrySource = nullptr);
        ShaderOpenGL(std::string shaderName, char* computeSource);
        ~ShaderOpenGL();

        void rename(char* _name);

        void loadVertexShader(char* source);
        void loadGeometryShader(char* source);
        void loadFragmentShader(char* source);
        void loadComputeShader(char* source);
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
        void setVector2D(const char* name, mathem::Vector2D vector);
        void setVector3D(const char* name, mathem::Vector3D vector);
        void setIntVector3D(const char* name, int x, int y, int z);
        void setVector4D(const char* name, mathem::Vector4D vector);
        void setUnsignedVector4D(const char* name, mathem::Vector4D vector);
        void setMatrix2x2(const char* name, mathem::Matrix2x2 matrix);
        void setMatrix3x3(const char* name, mathem::Matrix3x3 matrix);
        void setMatrix4x4(const char* name, mathem::Matrix4x4 matrix);
        void setMatrix4x4Array(const char* name, safety::SafeArray<mathem::Matrix4x4>* matrices);

    private:

        char* vertexFilename = nullptr;
        char* vertexSource = nullptr;
        int vertexCompileStatus = 0;
        char vertexInfoLog[512];

        char* geometryFilename = nullptr;
        char* geometrySource = nullptr;
        int geometryCompileStatus = 0;
        char geometryInfoLog[512];

        char* fragmentFilename = nullptr;
        char* fragmentSource = nullptr;
        int fragmentCompileStatus = 0;
        char fragmentInfoLog[512];

        char* computeFilename = nullptr;
        char* computeSource = nullptr;
        int computeCompileStatus = 0;
        char computeInfoLog[512];

        int programLinkStatus = 0;
        char programInfoLog[512];

        void deleteVertexShader();
        void deleteGeometryShader();
        void deleteFragmentShader();
        void deleteComputeShader();
        int compileAndLinkRegular();
        int compileAndLinkCompute();
    };
}

#endif