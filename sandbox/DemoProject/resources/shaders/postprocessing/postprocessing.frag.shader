#version 330 core

in vec2 Texcoord;
out vec4 outColor;

uniform float deltaTime = 0.0f;
uniform sampler2D mainCanvasTexture;

const float NOISE_AMPLITUDE = 1.0f / 255.0f;
float noise(vec2 screenUV)
{
    screenUV += vec2(1.0f - deltaTime, deltaTime) * 0.75f;
    return fract(12345.06789f * sin(dot(screenUV.xy, vec2(123.45f, 678.90f))));
}

void main()
{
    vec4 mainCanvasFragment = texture(mainCanvasTexture, Texcoord);
	outColor = mainCanvasFragment;

    // Add some noise to break up color banding
    float noiseValue = mix(-NOISE_AMPLITUDE, NOISE_AMPLITUDE, noise(Texcoord));
    outColor.rgb += noiseValue * (1.0f - vec3(0.3, 0.59, 0.11));
}

