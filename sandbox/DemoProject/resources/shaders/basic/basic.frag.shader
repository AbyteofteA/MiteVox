
#version 330 core
in vec2 Texcoord;
in vec3 Normal;
in vec3 Position;
in mat3 TangentBitangentNormal;

out vec4 outColor;

struct Material
{
	vec3 baseColor;
	float roughness;
	float metallicity;
	float specularExponent;

	bool hasAlbedoMap;
	bool hasMetallicRoughnessMap;
	bool hasNormalMap;

	bool hasReflectionMap;

	int illuminationModel;
};

#define ILLUMINATION_MODEL_CONSTANT_COLOR 0
#define ILLUMINATION_MODEL_DIFFUSE 1
#define ILLUMINATION_MODEL_SPECULAR 2
#define ILLUMINATION_MODEL_PBR 32

uniform sampler2D albedoMap;
uniform sampler2D metallicRoughnessMap;
uniform sampler2D normalMap;
uniform samplerCube reflectionMap;

uniform vec3 viewPos;
uniform Material material;

uniform vec3 ambientLight;

struct PointLight
{
	vec3 pos;
	vec3 color;
	float intensity;
	float range;
};
#define MAX_OF_POINT_LIGHTS 64
uniform int amountOfPointLights;
uniform PointLight pointLights[MAX_OF_POINT_LIGHTS];

// TODO: 
uniform vec3 fogColor = vec3(0.0f, 0.0f, 0.0f);
uniform float fogMinDistance = -1.0f;
uniform float fogMaxDistance = -1.0f;

float calculateAttenuation(float currentDistance, float range)
{
	// TODO: Recommended formula doesn't work for some reason
	//float attenuation = max(min(1.0f - pow(currentDistance / range, 4.0f), 1.0f), 0.0f) / pow(currentDistance, 2.0f);
	float attenuation = max(1.0f - pow(currentDistance / range, 2.0f), 0.0f);
	return attenuation;
}

vec3 calculate_PointLight(vec3 norm, vec3 viewDir, float roughnessFragment, float metallicFragment)
{
	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < amountOfPointLights; i++)
	{
		vec3 lightDir = pointLights[i].pos - Position;
		float distance = abs(length(lightDir));
		float attenuation = 1.0f;
		if (pointLights[i].range > 0.0f)
		{
			attenuation = pointLights[i].intensity * calculateAttenuation(distance, pointLights[i].range);
		}
		lightDir = normalize(lightDir);

		float dotNormLightdir = dot(norm, lightDir);

		float diff = max(dotNormLightdir, 0.0f);
		vec3 diffuse = diff * pointLights[i].color * roughnessFragment;

		vec3 specular = vec3(0.0f, 0.0f, 0.0f);
		vec3 reflectDir = reflect(-lightDir, norm);  
		if(dotNormLightdir > 0)
		{
			float spec = pow(max(dot(lightDir, reflectDir), 0.0f), material.specularExponent);
			specular = spec * pointLights[i].color * metallicFragment;
		}
		result += attenuation * (diffuse + specular);
	}
	return result;
}

void main()
{
	// Normal mapping
	vec3 norm = normalize(Normal);
	if (material.hasNormalMap)
	{
		norm = vec3(texture(normalMap, Texcoord));
		norm = normalize(norm * 2.0f - 1.0f);
		norm = normalize(TangentBitangentNormal * norm);
	}
	
	vec3 viewDir = normalize(viewPos - Position);

	// 
	vec3 albedoFragment = material.baseColor;
	if (material.hasAlbedoMap)
	{
		albedoFragment *= vec3(texture(albedoMap, Texcoord));
	}
	vec3 metallicRoughnessFragment;
	metallicRoughnessFragment.r = 0.0f;
	metallicRoughnessFragment.b = material.metallicity;
	metallicRoughnessFragment.g = material.roughness;
	if (material.hasMetallicRoughnessMap)
	{
		metallicRoughnessFragment *= vec3(texture(metallicRoughnessMap, Texcoord));
	}
	float metallicity = metallicRoughnessFragment.b;
	float roughness = metallicRoughnessFragment.g;
	/*vec4 reflectionMapFragment = vec4(0, 0, 0, 1);
	if(material.hasReflectionMap != 0)
	{
		vec3 reflectionDir = reflect(-viewDir, norm);
		//reflectionMapFragment = texture(reflectionMap, reflectionDir);
	}*/

	vec3 light = ambientLight;
	switch (material.illuminationModel)
	{
	case ILLUMINATION_MODEL_SPECULAR:
		light += calculate_PointLight(norm, viewDir, roughness, metallicity);
		break;

	case ILLUMINATION_MODEL_CONSTANT_COLOR:
	default:
		break;
	}
	outColor = vec4(albedoFragment * light, 1.0f);
}

