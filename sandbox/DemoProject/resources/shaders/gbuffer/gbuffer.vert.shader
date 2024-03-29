#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord_0;
layout(location = 3) in vec2 texcoord_1;
layout(location = 4) in vec4 color;
layout(location = 5) in uvec4 joints;
layout(location = 6) in vec4 weights;
layout(location = 7) in vec4 tangent;

out VertexDataFragment
{
	vec2 Texcoord;
	vec3 Normal;
	vec3 Position;
	mat3 TangentBitangentNormal;
} fragment;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 viewOrientationMatrix;
uniform mat4 projectionMatrix;

uniform int jointsCount;
uniform mat4 jointMatrices[32];

void main()
{
	mat4 poseMatrix = mat4(1.0);
	if (jointsCount > 0)
	{
		poseMatrix =
			weights.x * jointMatrices[joints.x] +
			weights.y * jointMatrices[joints.y] +
			weights.z * jointMatrices[joints.z] +
			weights.w * jointMatrices[joints.w];
	}

	mat4 poseModelViewMatrix = viewMatrix * modelMatrix * poseMatrix;

	fragment.Texcoord = texcoord_0;
	fragment.Normal = normalize(vec3(transpose(inverse(poseModelViewMatrix)) * vec4(normal, 1.0)));
	vec3 bitangent = cross(fragment.Normal, tangent.xyz);
	fragment.TangentBitangentNormal = mat3(tangent.xyz, bitangent, fragment.Normal);
	fragment.Position = vec3(poseModelViewMatrix * vec4(position, 1.0));
	gl_Position = projectionMatrix * vec4(fragment.Position, 1.0);
}

