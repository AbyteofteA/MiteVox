
#version 330 core

in vec3 position;

out vec3 Texcoord;

uniform mat4 viewProjectionMatrix;

void main()
{
	vec4 finalPos = viewProjectionMatrix * vec4(position, 1.0);
	Texcoord = vec3(position);
	gl_Position = finalPos.xyww;
}

