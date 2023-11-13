#version 330 core

in vec2 Texcoord;
out vec4 outColor;

uniform sampler2D mainCanvasTexture;

void main()
{
    vec4 mainCanvasFragment = texture(mainCanvasTexture, Texcoord);
	outColor = mainCanvasFragment;
}

