#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 48) out;

#define SHADOW_MAP_PACK_SIZE 16
uniform int shadowMapsCount = 0;
uniform mat4 lightMatrices[SHADOW_MAP_PACK_SIZE];

void main()
{
    for (int shadowMap = 0; shadowMap < shadowMapsCount; ++shadowMap)
    {
        gl_Layer = shadowMap;
        for (int i = 0; i < gl_in.length(); ++i)
        {
            gl_Position = lightMatrices[shadowMap] * gl_in[i].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}