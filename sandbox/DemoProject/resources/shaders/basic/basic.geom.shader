#version 330 core

in VertexDataGeometry
{
    vec2 Texcoord;
    vec3 Normal;
    vec3 Position;
    mat3 TangentBitangentNormal;
} vertex[];

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out VertexDataFragment
{
    vec2 Texcoord;
    vec3 Normal;
    vec3 Position;
    mat3 TangentBitangentNormal;
} fragment;

void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
        fragment.Texcoord = vertex[i].Texcoord;
        fragment.Normal = vertex[i].Normal;
        fragment.Position = vertex[i].Position;
        fragment.TangentBitangentNormal = vertex[i].TangentBitangentNormal;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}

