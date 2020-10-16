
#version 330 core
in vec3 position;
in vec2 texcoord;
in vec3 normal;

out vec2 Texcoord;
out vec3 Normal;
out vec3 Position;

uniform mat4 positionTransform;
uniform mat4 model;

void main()
{
	Texcoord = texcoord;
	Normal = vec3(transpose(inverse(model)) * vec4(normal, 1.0));
	Position = vec3(model * vec4(position, 1.0));
	gl_Position = (positionTransform * vec4(position, 1.0));
}

