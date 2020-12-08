
#version 330 core
in vec2 Texcoord;
in vec3 Normal;
in vec3 Position;

out vec4 outColor;

struct Material
{
	vec3 albedo;
	vec3 roughness;
	vec3 metallicity;
	float specularExponent;
};

uniform vec3 viewPos;

uniform Material material;

uniform int hasReflectionMap = 0;
uniform samplerCube reflectionMap;

uniform int hasAlbedoMap = 0;
uniform sampler2D albedoMap;

uniform int hasRoughnessMap = 0;
uniform sampler2D roughnessMap;

uniform int hasMetallicMap = 0;
uniform sampler2D metallicMap;


uniform struct
{
	vec3 color;
} ambientLight;


struct DirectionalLight
{
	vec3 direction;
	vec3 color;
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
};
#define MAX_OF_SPOT_LIGHTS 64
uniform int amountOfSpotLights;
uniform SpotLight spotLights[MAX_OF_SPOT_LIGHTS];


vec3 calculate_DirectionalLight(vec3 norm, vec3 viewDir, vec3 roughnessFragment, vec3 metallicFragment)
{
	vec3 result = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < amountOfDirectionalLights; i++)
	{
		vec3 lightDir = normalize(directionalLights[i].direction);

		float dotNormLightdir = dot(norm, lightDir);

		float diff = max(dotNormLightdir, 0.0);
		vec3 diffuse = material.roughness * diff * directionalLights[i].color * roughnessFragment;

		vec3 specular = vec3(0.0, 0.0, 0.0);
		vec3 reflectDir = reflect(-lightDir, norm);  
		if(dotNormLightdir > 0)
		{
			float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.specularExponent);
			specular = material.metallicity * spec * directionalLights[i].color * metallicFragment;
		}
		result += diffuse + specular;
	}
	return result;
}

vec3 calculate_PointLight(vec3 norm, vec3 viewDir, vec3 roughnessFragment, vec3 metallicFragment)
{
	vec3 result = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < amountOfPointLights; i++)
	{
		vec3 lightDir = pointLights[i].pos - Position;
		float distance = abs(length(lightDir));
		float attenuation = 
			1 / (pointLights[i].constant + 
			pointLights[i].linear * distance + 
			pointLights[i].quadratic * distance * distance);
		lightDir = normalize(lightDir);

		float dotNormLightdir = dot(norm, lightDir);

		float diff = max(dotNormLightdir, 0.0);
		vec3 diffuse = material.roughness * diff * pointLights[i].color * roughnessFragment;

		vec3 specular = vec3(0.0, 0.0, 0.0);
		vec3 reflectDir = reflect(-lightDir, norm);  
		if(dotNormLightdir > 0)
		{
			float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.specularExponent);
			specular = material.metallicity * spec * pointLights[i].color * metallicFragment;
		}
		result += attenuation * (diffuse + specular);
	}
	return result;
}

vec3 calculate_SpotLight(vec3 norm, vec3 viewDir, vec3 roughnessFragment, vec3 metallicFragment)
{
	vec3 result = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < amountOfSpotLights; i++)
	{
		vec3 lightDir = spotLights[i].pos - Position;
		float distance = abs(length(lightDir));
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
		vec3 diffuse = material.roughness * diff * spotLights[i].color * roughnessFragment;

		vec3 specular = vec3(0.0, 0.0, 0.0);
		vec3 reflectDir = reflect(-lightDir, norm);  
		if(dotNormLightdir > 0)
		{
			float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.specularExponent);
			specular = material.metallicity * spec * spotLights[i].color * metallicFragment;
		}
		result += intensity * attenuation * (diffuse + specular);
		
	}
	return result;
}


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - Position);

	vec3 albedoFragment = vec3(texture(albedoMap, Texcoord));
	vec3 roughnessFragment = vec3(texture(roughnessMap, Texcoord));
	vec3 metallicFragment = vec3(texture(metallicMap, Texcoord));

	vec4 reflectionMapFragment = vec4(0, 0, 0, 1);
	if(hasReflectionMap != 0)
	{
		vec3 reflectionDir = reflect(-viewDir, norm);
		//reflectionMapFragment = texture(reflectionMap, reflectionDir);
	}

	vec3 result = material.albedo * albedoFragment +
				calculate_DirectionalLight(norm, viewDir, roughnessFragment, metallicFragment) +
				calculate_PointLight(norm, viewDir, roughnessFragment, metallicFragment) + 
				calculate_SpotLight(norm, viewDir, roughnessFragment, metallicFragment);
	outColor = vec4(result, 1.0);
}

