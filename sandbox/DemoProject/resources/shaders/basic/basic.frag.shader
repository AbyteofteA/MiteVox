#version 330 core
in vec2 Texcoord;
in vec3 Normal;
in vec3 Position;
in mat3 TangentBitangentNormal;

out vec4 outColor;

// Shader settings
uniform float gammaCorrection = 1.5f;

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

#define ILLUMINATION_MODEL_NONE 0
#define ILLUMINATION_MODEL_UNLIT 1
#define ILLUMINATION_MODEL_PHONG 2
#define ILLUMINATION_MODEL_PBR 32

uniform sampler2D albedoMap;
uniform sampler2D metallicRoughnessMap;
uniform sampler2D normalMap;
uniform sampler2D occlusionMap;
uniform sampler2D emissiveMap;

uniform vec3 viewPos;
uniform Material material;

uniform vec3 ambientLight;

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
	float intensity;
};
#define MAX_OF_DIRECTIONAL_LIGHTS 64
uniform int amountOfDirectionalLights;
uniform DirectionalLight directionalLights[MAX_OF_DIRECTIONAL_LIGHTS];

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

struct SpotLight
{
	vec3 pos;
	vec3 direction;
	float innerConeAngle;
	float outerConeAngle;
	vec3 color;
	float intensity;
	float range;
};
#define MAX_OF_SPOT_LIGHTS 64
uniform int amountOfSpotLights;
uniform SpotLight spotLights[MAX_OF_SPOT_LIGHTS];

uniform vec3 fogColor = vec3(0.05f, 0.05f, 0.05f);
uniform float fogMinDistance = 20.0f;
uniform float fogMaxDistance = 50.0f;

#define PI 3.1415926535897932384626433832795
#define ALMOST_ZERO 0.000001

vec3 calculateLightSourcePhong(
	vec3 albedoFragment,
	float roughness,
	float metallicity,
	vec3 norm,
	vec3 viewDir,
	vec3 lightDir,
	vec3 lightColor)
{
	float dotNormLightDir = max(dot(norm, lightDir), 0.0f);
	vec3 reflectDir = reflect(-lightDir, norm);
	float dotViewReflectDir = max(dot(viewDir, reflectDir), 0.0f);

	vec3 diffuse = albedoFragment * lightColor;
	float smoothness = (1.0f - roughness);
	vec3 specular = lightColor * pow(dotViewReflectDir, clamp(smoothness * 256.0f, 2.0f, 256.0f)) * metallicity;

	return (diffuse + specular) * dotNormLightDir;
}

/// <summary>
/// Trowbridge-Reitz GGX normal distribution function
/// </summary>
float normalDistributionFunction(vec3 norm, vec3 halfVector, float roughness)
{
	float roughness4 = pow(roughness, 4.0f); // originally ^2.0f
	float dotNormHalf = max(dot(norm, halfVector), 0.0f);
	float denominator = (dotNormHalf * dotNormHalf * (roughness4 - 1.0f) + 1.0f);
	return roughness4 / (PI * denominator * denominator);
}

/// <summary>
/// Schlick GGX geometry function
/// </summary>
float geometryFunction(float dotProduct, float K)
{
	return dotProduct / (dotProduct * (1.0f - K) + K);
}

/// <summary>
/// Fresnel-Schlick equation
/// </summary>
vec3 specularFractionFunction(vec3 viewDir, vec3 halfVector, vec3 F0, float roughness)
{
	float dotViewDirHalf = max(dot(viewDir, halfVector), 0.0f);
	return F0 + (1.0f - F0) * pow(clamp(1.0f - dotViewDirHalf, 0.0f, 1.0f), 5.0f);
}

/// <summary>
/// Cook-Torrance bidirectional reflective distribution function (BRDF)
/// </summary>
vec3 calculateLightSourcePBR(
	vec3 albedoFragment, 
	float roughness, 
	float metallicity, 
	vec3 F0, 
	float K, 
	vec3 norm, 
	vec3 viewDir, 
	float dotNormViewDir, 
	vec3 lightDir, 
	vec3 lightColor)
{
	float dotNormLightDir = max(dot(norm, lightDir), 0.0f);
	vec3 halfVector = normalize(lightDir + viewDir);

	// Reflection fraction
	vec3 specularFraction = specularFractionFunction(viewDir, halfVector, F0, roughness);

	// Refraction fraction
	vec3 diffuseFraction = vec3(1.0f) - specularFraction;
	diffuseFraction *= (1.0f - metallicity);

	// Diffuse component
	vec3 diffuseBRDF = diffuseFraction * albedoFragment / PI;

	// Specular component
	float microfacetsAlignment = normalDistributionFunction(norm, halfVector, roughness);
	float microfacetShadowing = geometryFunction(dotNormLightDir, K) * geometryFunction(dotNormViewDir, K);
	vec3 specularBRDF = (specularFraction * microfacetsAlignment) / (4.0f * dotNormLightDir * dotNormViewDir + ALMOST_ZERO);

	return (diffuseBRDF + specularBRDF) * lightColor * dotNormLightDir;
}

vec3 calculateLightSource(
	vec3 albedoFragment,
	float roughness,
	float metallicity,
	vec3 F0,
	float K,
	vec3 norm,
	vec3 viewDir,
	float dotNormViewDir,
	vec3 lightDir,
	vec3 lightColor)
{
	switch (material.illuminationModel)
	{
	case ILLUMINATION_MODEL_PHONG:
		return calculateLightSourcePhong(albedoFragment, roughness, metallicity, norm, viewDir, lightDir, lightColor);

	case ILLUMINATION_MODEL_PBR:
		return calculateLightSourcePBR(albedoFragment, roughness, metallicity, F0, K, norm, viewDir, dotNormViewDir, lightDir, lightColor);

	case ILLUMINATION_MODEL_UNLIT:
	default:
		break;
	}
	return vec3(0.0f);
}

float calculateAttenuation(float currentDistance, float range)
{
	// TODO: Recommended formula doesn't work for some reason
	//float attenuation = max(min(1.0f - pow(currentDistance / range, 4.0f), 1.0f), 0.0f) / pow(currentDistance, 2.0f);
	float attenuation = max(1.0f - pow(currentDistance / range, 2.0f), 0.0f);
	return attenuation;
}

vec3 calculateLighting(vec3 albedoFragment, float roughness, float metallicity, vec3 norm, float occlusion)
{
	// Constants
	vec3 viewDir = normalize(viewPos - Position);
	float roughnessSquared = pow(roughness, 2.0f); // originally ^1.0f
	float K = (roughnessSquared + 1.0f) * (roughnessSquared + 1.0f) / 8.0f;
	vec3 F0 = vec3(0.04f);
	F0 = mix(F0, albedoFragment, metallicity);
	float dotNormViewDir = max(dot(norm, viewDir), 0.0f);

	// Ambient light
	vec3 result = ambientLight * occlusion * albedoFragment;

	// Directional lights
	for (int i = 0; i < amountOfDirectionalLights; i++)
	{
		vec3 lightDir = -directionalLights[i].direction;
		vec3 lightColor = directionalLights[i].color * directionalLights[i].intensity;
		lightDir = normalize(lightDir);
		result += calculateLightSource(albedoFragment, roughness, metallicity, F0, K, norm, viewDir, dotNormViewDir, lightDir, lightColor);
	}

	// Point lights
	for (int i = 0; i < amountOfPointLights; i++)
	{
		vec3 lightDir = pointLights[i].pos - Position;
		float distance = abs(length(lightDir));
		float attenuation = 0.0f;
		if (pointLights[i].range > 0.0f)
		{
			attenuation = calculateAttenuation(distance, pointLights[i].range);
		}
		vec3 lightColor = pointLights[i].color * pointLights[i].intensity * attenuation;
		lightDir = normalize(lightDir);
		
		result += calculateLightSource(albedoFragment, roughness, metallicity, F0, K, norm, viewDir, dotNormViewDir, lightDir, lightColor);
	}

	// Spot lights
	for (int i = 0; i < amountOfSpotLights; i++)
	{
		vec3 lightDir = spotLights[i].pos - Position;
		float distance = abs(length(lightDir));
		float attenuation = 0.0f;
		if (spotLights[i].range > 0.0f)
		{
			attenuation = calculateAttenuation(distance, spotLights[i].range);
		}
		vec3 lightColor = spotLights[i].color * spotLights[i].intensity * attenuation;
		lightDir = normalize(lightDir);
	
		float theta = dot(lightDir, normalize(-spotLights[i].direction));
		
		if (theta > cos(spotLights[i].outerConeAngle))
		{
			float epsilon = cos(spotLights[i].innerConeAngle) - cos(spotLights[i].outerConeAngle);
			float spotLightIntensity = clamp((theta - cos(spotLights[i].outerConeAngle)) / epsilon, 0.0f, 1.0f);
			lightColor *= spotLightIntensity;
			result += calculateLightSource(albedoFragment, roughness, metallicity, F0, K, norm, viewDir, dotNormViewDir, lightDir, lightColor);
		}
	}

	return result;
}

float calculateLinearFog()
{
	float distance = length(Position - viewPos) - fogMinDistance;
	float fogRange = fogMaxDistance - fogMinDistance;
	float fogFactor = clamp(distance / fogRange, 0.0f, 1.0f);
	return fogFactor;
}

void main()
{
	// Albedo
	vec3 albedoFragment = material.baseColor;
	if (material.hasAlbedoMap)
	{
		albedoFragment *= vec3(texture(albedoMap, Texcoord));
	}

	// Normal map
	vec3 norm = normalize(Normal);
	if (material.hasNormalMap)
	{
		norm = vec3(texture(normalMap, Texcoord));
		norm = normalize(norm * 2.0f - 1.0f);
		norm = normalize(TangentBitangentNormal * norm);
	}
	
	// Metallicity and roughness
	vec3 metallicRoughnessFragment;
	metallicRoughnessFragment.r = 0.0f;
	metallicRoughnessFragment.b = material.metallicity;
	metallicRoughnessFragment.g = material.roughness;
	if (material.hasMetallicRoughnessMap)
	{
		metallicRoughnessFragment *= vec3(texture(metallicRoughnessMap, Texcoord));
	}
	float metallicity = metallicRoughnessFragment.b;
	float roughness = mix(0.05f, 0.95f, metallicRoughnessFragment.g);

	// Occlusion
	float occlusion = 1.0f;
	if (material.hasOcclusionMap)
	{
		occlusion = texture(occlusionMap, Texcoord).r;
	}

	// Emissive map
	vec3 emissiveFragment = vec3(0.0f);
	if (material.hasEmissiveMap)
	{
		emissiveFragment = vec3(texture(emissiveMap, Texcoord));
	}
	else
	{
		emissiveFragment = material.emission;
	}

	outColor = vec4(0.0f);
	if (material.illuminationModel == ILLUMINATION_MODEL_UNLIT)
	{
		outColor = vec4(albedoFragment, 1.0f);
	}
	else
	{
		outColor = vec4(calculateLighting(albedoFragment, roughness, metallicity, norm, occlusion), 1.0f);
		//outColor += vec4(emissiveFragment, 1.0f); // TODO: add light bloom
		outColor.rgb = outColor.rgb / (outColor.rgb + vec3(1.0f));
		outColor.rgb = pow(outColor.rgb, vec3(1.0f / gammaCorrection));
	}

	// Linear fog
	float fogFactor = calculateLinearFog();
	outColor.rgb = mix(outColor.rgb, fogColor, fogFactor);
}

