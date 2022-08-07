
#version 330 core
in vec2 Texcoord;
in vec3 Normal;
in vec3 Position;

out vec4 outColor;

struct Material
{
	vec3 baseColor;
	float roughness;
	float metallicity;
	float specularExponent;

	bool hasAlbedoMap;
	bool hasMetallicRoughnessMap;
	bool hasReflectionMap;

	int illuminationModel;
};

#define ILLUMINATION_MODEL_CONSTANT_COLOR 0
#define ILLUMINATION_MODEL_DIFFUSE 1
#define ILLUMINATION_MODEL_SPECULAR 2
#define ILLUMINATION_MODEL_PBR 32

uniform sampler2D albedoMap;
uniform sampler2D metallicRoughnessMap;
uniform samplerCube reflectionMap;

uniform vec3 viewPos;
uniform Material material;


struct DirectionalLight
{
	vec3 direction;
	vec3 color;

	float intensity;
	float range;
};
#define MAX_OF_DIRECTIONAL_LIGHTS 8
uniform int amountOfDirectionalLights;
uniform DirectionalLight directionalLights[MAX_OF_DIRECTIONAL_LIGHTS];

struct PointLight
{
	vec3 pos;
	vec3 color;

	float constant;
    float linear;
    float quadratic;

	float intensity;
	float range;
};
#define MAX_OF_POINT_LIGHTS 64
uniform int amountOfPointLights;
uniform PointLight pointLights[MAX_OF_POINT_LIGHTS];

struct SpotLight
{
	vec3 pos;
	vec3 direction;
	float angle;
	vec3 color;

	float constant;
    float linear;
    float quadratic;

	float intensity;
	float range;
};
#define MAX_OF_SPOT_LIGHTS 64
uniform int amountOfSpotLights;
uniform SpotLight spotLights[MAX_OF_SPOT_LIGHTS];

float calculateAttenuation(float currentDistance, float range)
{
	//float attenuation = max(min(1.0 - pow(currentDistance / range, 4.0), 1.0), 0.0) / pow(currentDistance, 2.0);
	float attenuation = max(1.0 - pow(currentDistance / range, 2.0), 0.0);
	return attenuation;
}

vec3 calculate_DirectionalLight(vec3 norm, vec3 viewDir, float roughnessFragment, float metallicFragment)
{
	vec3 result = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < amountOfDirectionalLights; i++)
	{
		vec3 lightDir = normalize(directionalLights[i].direction);
		float distance = abs(length(lightDir));
		float attenuation = 1;// calculateAttenuation(distance, directionalLights[i].range);

		float dotNormLightdir = dot(norm, lightDir);

		float diff = max(dotNormLightdir, 0.0);
		vec3 diffuse = diff * directionalLights[i].color * roughnessFragment;

		vec3 specular = vec3(0.0, 0.0, 0.0);
		vec3 reflectDir = reflect(-lightDir, norm);  
		if(dotNormLightdir > 0)
		{
			float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.specularExponent);
			specular = spec * directionalLights[i].color * metallicFragment;
		}
		result += attenuation * (diffuse + specular);
	}
	return result;
}

vec3 calculate_PointLight(vec3 norm, vec3 viewDir, float roughnessFragment, float metallicFragment)
{
	vec3 result = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < amountOfPointLights; i++)
	{
		vec3 lightDir = pointLights[i].pos - Position;
		float distance = abs(length(lightDir));
		//float attenuation = calculateAttenuation(distance, pointLights[i].range);
		float attenuation =
			1 / (pointLights[i].constant + 
			pointLights[i].linear * distance + 
			pointLights[i].quadratic * distance * distance);
		lightDir = normalize(lightDir);

		float dotNormLightdir = dot(norm, lightDir);

		float diff = max(dotNormLightdir, 0.0);
		vec3 diffuse = diff * pointLights[i].color * roughnessFragment;

		vec3 specular = vec3(0.0, 0.0, 0.0);
		vec3 reflectDir = reflect(-lightDir, norm);  
		if(dotNormLightdir > 0)
		{
			float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.specularExponent);
			specular = spec * pointLights[i].color * metallicFragment;
		}
		result += attenuation * (diffuse + specular);
	}
	return result;
}

vec3 calculate_SpotLight(vec3 norm, vec3 viewDir, float roughnessFragment, float metallicFragment)
{
	vec3 result = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < amountOfSpotLights; i++)
	{
		vec3 lightDir = spotLights[i].pos - Position;
		float distance = abs(length(lightDir));
		//float attenuation = calculateAttenuation(distance, spotLights[i].range);
		float attenuation =
			1 / (spotLights[i].constant + 
			spotLights[i].linear * distance + 
			spotLights[i].quadratic * distance * distance);
		lightDir = normalize(lightDir);

		// Angle calculations.
		float cutOff = cos(radians(spotLights[i].angle * 0.5));
		float outerCutOff = cos(radians(spotLights[i].angle));
		float theta = dot(lightDir, -normalize(spotLights[i].direction));
		float epsilon = cutOff - outerCutOff;
		float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

		// Light calculations.
		float dotNormLightdir = dot(norm, lightDir);

		float diff = max(dotNormLightdir, 0.0);
		vec3 diffuse = diff * spotLights[i].color * roughnessFragment;

		vec3 specular = vec3(0.0, 0.0, 0.0);
		vec3 reflectDir = reflect(-lightDir, norm);  
		if(dotNormLightdir > 0)
		{
			float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.specularExponent);
			specular = spec * spotLights[i].color * metallicFragment;
		}
		result += intensity * attenuation * (diffuse + specular);
		
	}
	return result;
}


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - Position);

	vec3 albedoFragment = material.baseColor;
	if (material.hasAlbedoMap)
	{
		albedoFragment *= vec3(texture(albedoMap, Texcoord));
	}
	vec3 metallicRoughnessFragment;
	metallicRoughnessFragment.r = 0.0;
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

	vec3 result = vec3(1.0, 0.0, 1.0);
	switch (material.illuminationModel)
	{
	case ILLUMINATION_MODEL_SPECULAR:
		result = albedoFragment *
			(calculate_DirectionalLight(norm, viewDir, roughness, metallicity) +
				calculate_PointLight(norm, viewDir, roughness, metallicity) +
				calculate_SpotLight(norm, viewDir, roughness, metallicity));
		break;

	case ILLUMINATION_MODEL_CONSTANT_COLOR:
	default:
		result = albedoFragment;
		break;
	}
	outColor = vec4(result, 1.0);
}

