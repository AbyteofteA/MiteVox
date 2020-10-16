
#version 330 core
in vec2 Texcoord;
in vec3 Normal;
in vec3 Position;

out vec4 outColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specularExponent;
};

uniform vec3 viewPos;

uniform Material material;

int hasAlbedoMap;
uniform sampler2D albedoMap;

int hasRoughnessMap;
uniform sampler2D roughnessMap;

int hasMetallicMap;
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


vec3 calculate_DirectionalLight(vec3 norm, vec3 viewDir, vec3 textureFragment, vec3 specularFragment)
{
	vec3 result = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < amountOfDirectionalLights; i++)
	{
		vec3 lightDir = normalize(directionalLights[i].direction);

		float dotNormLightdir = dot(norm, lightDir);

		float diff = max(dotNormLightdir, 0.0);
		vec3 diffuse = material.diffuse * diff * directionalLights[i].color * textureFragment;

		vec3 specular = vec3(0.0, 0.0, 0.0);
		vec3 reflectDir = reflect(-lightDir, norm);  
		if(dotNormLightdir > 0)
		{
			float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.specularExponent);
			specular = material.specular * spec * directionalLights[i].color * specularFragment;
		}
		result += diffuse + specular;
	}
	return result;
}

vec3 calculate_PointLight(vec3 norm, vec3 viewDir, vec3 textureFragment, vec3 specularFragment)
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
		vec3 diffuse = material.diffuse * diff * pointLights[i].color * textureFragment;

		vec3 specular = vec3(0.0, 0.0, 0.0);
		vec3 reflectDir = reflect(-lightDir, norm);  
		if(dotNormLightdir > 0)
		{
			float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.specularExponent);
			specular = material.specular * spec * pointLights[i].color * specularFragment;
		}
		result += attenuation * (diffuse + specular);
	}
	return result;
}

vec3 calculate_SpotLight(vec3 norm, vec3 viewDir, vec3 textureFragment, vec3 specularFragment)
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
		float theta = dot(lightDir, normalize(-spotLights[i].direction));
		float epsilon = cutOff - outerCutOff;
		float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

		// Light calculations.
		float dotNormLightdir = dot(norm, lightDir);

		float diff = max(dotNormLightdir, 0.0);
		vec3 diffuse = material.diffuse * diff * spotLights[i].color * textureFragment;

		vec3 specular = vec3(0.0, 0.0, 0.0);
		vec3 reflectDir = reflect(-lightDir, norm);  
		if(dotNormLightdir > 0)
		{
			float spec = pow(max(dot(lightDir, reflectDir), 0.0), material.specularExponent);
			specular = material.specular * spec * spotLights[i].color * specularFragment;
		}
		result += intensity * attenuation * (diffuse + specular);
		
	}
	return result;
}


void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - Position);

	vec3 textureFragment = vec3(texture(albedoMap, Texcoord));
	vec3 specularFragment = vec3(texture(metallicMap, Texcoord));

	vec3 result = material.ambient +
				calculate_DirectionalLight(norm, viewDir, textureFragment, specularFragment) +
				calculate_PointLight(norm, viewDir, textureFragment, specularFragment) + 
				calculate_SpotLight(norm, viewDir, textureFragment, specularFragment);
	outColor = vec4(result, 1.0);
}

