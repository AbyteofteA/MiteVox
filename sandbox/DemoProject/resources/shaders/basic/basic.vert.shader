
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord_0;
layout (location = 3) in vec2 texcoord_1;
layout (location = 4) in vec4 color;
layout (location = 5) in uvec4 joints;
layout (location = 6) in vec4 weights;
layout (location = 7) in vec4 tangent;

out vec2 Texcoord;
out vec3 Normal;
out vec3 Position;

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;

uniform int jointsCount;
uniform mat4 jointMatrices[64];

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

	mat4 poseModelMatrix = modelMatrix * poseMatrix;
	mat4 poseModelViewProjection = viewProjectionMatrix * poseModelMatrix;

	Texcoord = texcoord_0;
	Normal = vec3(transpose(inverse(poseModelMatrix)) * vec4(normal, 1.0));
	Position = vec3(poseModelMatrix * vec4(position, 1.0));
	gl_Position = (poseModelViewProjection * vec4(position, 1.0));
}

