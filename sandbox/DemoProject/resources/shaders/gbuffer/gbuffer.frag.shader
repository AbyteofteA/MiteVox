#version 330 core

in VertexDataFragment
{
	vec2 Texcoord;
	vec3 Normal;
	vec3 Position;
	mat3 TangentBitangentNormal;
} fragment;

layout (location = 0) out vec4 outPosition;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;
layout (location = 3) out vec4 outMaterial;

struct Material
{
	vec3 baseColor;
	float roughness;
	float metallicity;
	float specularExponent;
	vec3 emission;

	bool hasAlbedoMap;
	bool hasMetallicRoughnessMap;
	bool hasNormalMap;
	bool hasOcclusionMap;
	bool hasEmissiveMap;

	int illuminationModel;
};

uniform sampler2D albedoMap;
uniform sampler2D metallicRoughnessMap;
uniform sampler2D normalMap;
uniform sampler2D occlusionMap;
uniform sampler2D emissiveMap;

uniform Material material;

void main()
{
	// Albedo
	vec4 albedoFragment = vec4(material.baseColor, 1.0f);
	if (material.hasAlbedoMap)
	{
		albedoFragment = texture(albedoMap, fragment.Texcoord);
	}

	if (albedoFragment.a < 0.05f)
	{
		discard;
	}

	// Normal map
	vec3 normal = normalize(fragment.Normal);
	if (material.hasNormalMap)
	{
		normal = vec3(texture(normalMap, fragment.Texcoord));
		normal = normalize(normal * 2.0f - 1.0f);
		normal = normalize(fragment.TangentBitangentNormal * normal);
	}
	
	// Emissive map
	vec3 emissiveFragment = vec3(0.0f);
	if (material.hasEmissiveMap)
	{
		emissiveFragment = vec3(texture(emissiveMap, fragment.Texcoord));
	}
	else
	{
		emissiveFragment = material.emission;
	}
	
	// Metallicity and roughness
	vec3 metallicRoughnessFragment;
	metallicRoughnessFragment.r = 0.0f;
    metallicRoughnessFragment.g = material.roughness;
	metallicRoughnessFragment.b = material.metallicity;
	if (material.hasMetallicRoughnessMap)
	{
		metallicRoughnessFragment = vec3(texture(metallicRoughnessMap, fragment.Texcoord));
	}
    float roughness = mix(0.01f, 0.99f, metallicRoughnessFragment.g);
	float metallicity = metallicRoughnessFragment.b;

	// Occlusion
	float occlusion = 1.0f;
	if (material.hasOcclusionMap)
	{
		occlusion = texture(occlusionMap, fragment.Texcoord).r;
	}

    // Output
    outPosition = vec4(fragment.Position, emissiveFragment.r);
    outNormal = vec4(normal, emissiveFragment.g);
    outAlbedo = vec4(albedoFragment.rgb, occlusion);
    outMaterial = vec4(1.0f, roughness, metallicity, emissiveFragment.b);
}

