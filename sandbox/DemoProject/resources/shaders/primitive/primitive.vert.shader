
#version 330 core
in vec3 position;
in vec4 color;

uniform mat4 viewProjectionMatrix;

out vec4 Color;

void main()
{
	Color = color;
	gl_Position = viewProjectionMatrix * vec4(position, 1.0);
}

