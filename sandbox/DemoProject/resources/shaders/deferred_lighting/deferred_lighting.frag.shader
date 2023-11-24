#version 330 core

in vec2 Texcoord;
out vec4 outColor;

#define ILLUMINATION_MODEL_NONE 0
#define ILLUMINATION_MODEL_UNLIT 1
#define ILLUMINATION_MODEL_PHONG 2
#define ILLUMINATION_MODEL_PBR 32

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;
uniform sampler2D materialTexture;

uniform float nearPlane = 0.001f;
uniform float farPlane = 1500.0f;
uniform vec3 viewPos;
uniform mat4 viewMatrix;
uniform mat4 viewOrientationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 inverseViewMatrix;
uniform mat4 inverseViewOrientationMatrix;
uniform mat4 inverseProjectionMatrix;

uniform vec3 ambientLight;

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
	float intensity;
};
#define MAX_OF_DIRECTIONAL_LIGHTS 16
uniform int amountOfDirectionalLights;
uniform DirectionalLight directionalLights[MAX_OF_DIRECTIONAL_LIGHTS];

struct PointLight
{
	vec3 pos;
	vec3 color;
	float intensity;
	float range;
};
#define MAX_OF_POINT_LIGHTS 16
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
#define MAX_OF_SPOT_LIGHTS 16
uniform int amountOfSpotLights;
uniform SpotLight spotLights[MAX_OF_SPOT_LIGHTS];

#define SHADOW_MAP_PACK_SIZE 16
uniform sampler2DArray shadowMapPack;
uniform mat4 lightMatrices[SHADOW_MAP_PACK_SIZE];

// Precomputed properties

vec3 position = vec3(0.0f);
vec3 positionWorld = vec3(0.0f);
vec4 albedoFragment = vec4(0.0f);
vec3 norm = vec3(0.0f);
vec3 normWorld = vec3(0.0f);
float metallicity = 0.0f;
float roughness = 1.0f;
float occlusion = 1.0f;
vec3 emissiveFragment = vec3(0.0f);

vec3 viewDir = vec3(0.0f);
vec3 viewDirWorld = vec3(0.0f);
float roughnessSquared = 0.0f;
float K = 0.0f;
vec3 F0 = vec3(0.0f);
float dotNormViewDir = 0.0f;
float shadowMapTextureSize = 0.0f;

#define PI 3.1415926535897932384626433832795
#define ALMOST_ZERO 0.000001

vec3 calculateLightSourcePhong(
	vec3 albedoFragment,
	float roughness,
	float metallicity,
	vec3 norm,
	vec3 lightDir,
	vec3 lightColor)
{
	float dotNormLightDir = max(dot(norm, lightDir), 0.0f);
	vec3 reflectDir = reflect(-lightDir, norm);
	float dotViewReflectDir = max(dot(viewDirWorld, reflectDir), 0.0f);

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
	float dotNormViewDir, 
	vec3 lightDir, 
	vec3 lightColor)
{
	float dotNormLightDir = max(dot(norm, lightDir), 0.0f);
	vec3 halfVector = normalize(lightDir + viewDirWorld);

    vec3 specularFraction = 
        specularFractionFunction(viewDirWorld, halfVector, F0, roughness); // Reflection fraction
	vec3 diffuseFraction = vec3(1.0f) - specularFraction; // Refraction fraction
	diffuseFraction *= (1.0f - metallicity);
    
    // Diffuse component
	vec3 diffuseBRDF = diffuseFraction * albedoFragment.rgb / PI;

	// Specular component
	float microfacetsAlignment = normalDistributionFunction(norm, halfVector, roughness);
	float microfacetShadowing = geometryFunction(dotNormLightDir, K) * geometryFunction(dotNormViewDir, K);
	vec3 specularBRDF = (specularFraction * microfacetsAlignment * microfacetShadowing) / (4.0f * dotNormLightDir * dotNormViewDir + ALMOST_ZERO);

	return (diffuseBRDF + specularBRDF) * lightColor * dotNormLightDir;
}

vec3 calculateLightSource(
	vec3 albedoFragment,
	float roughness,
	float metallicity,
	vec3 F0,
	float K,
	vec3 norm,
	float dotNormViewDir,
	vec3 lightDir,
	vec3 lightColor)
{
    int illuminationModel = ILLUMINATION_MODEL_PBR;
	switch (illuminationModel)
	{
	case ILLUMINATION_MODEL_PHONG:
		return calculateLightSourcePhong(albedoFragment, roughness, metallicity, norm, lightDir, lightColor);

	case ILLUMINATION_MODEL_PBR:
		return calculateLightSourcePBR(albedoFragment, roughness, metallicity, F0, K, norm, dotNormViewDir, lightDir, lightColor);

	case ILLUMINATION_MODEL_UNLIT:
		return albedoFragment;

	default:
		break;
	}
	return vec3(0.0f);
}

float calculateAttenuation(float currentDistance, float range)
{
	//return max(min(1.0f - pow(currentDistance / range, 4.0f), 1.0f), 0.0f) / pow(currentDistance, 2.0f);
	//return 0.5f * cos(PI * clamp(currentDistance / range, 0.0f, 1.0f)) + 0.5;
	return pow(1.0f - sin(0.5f * PI * clamp(currentDistance / range, 0.0f, 1.0f)), 1.5f);
}

float calculateSpotShadow(int lightIndex)
{
	float shadowFraction = 0.0f;
	vec4 fragmentLightSpace = lightMatrices[lightIndex] * vec4(positionWorld, 1.0f);
	fragmentLightSpace.xyz /= fragmentLightSpace.w;
	if (fragmentLightSpace.z <= 1.0f)
	{
		fragmentLightSpace.xyz = fragmentLightSpace.xyz * 0.5f + 0.5f;
		float pixelSize = 1.0f / shadowMapTextureSize;
		int radius = 2;
		for (int x = -radius; x <= radius; ++x)
		{
			for (int y = -radius; y <= radius; ++y)
			{
				vec3 samplePos;
				samplePos.xy = fragmentLightSpace.xy + vec2(x, y) * pixelSize;
				samplePos.z = lightIndex;
				float closestDepth = texture(shadowMapPack, samplePos).r;
				float currentDepth = fragmentLightSpace.z;
				if (currentDepth > closestDepth)
				{
					shadowFraction += 1.0f;
				}
			}
		}
		shadowFraction /= 25.0f;
	}
	return 1.0f - shadowFraction;
}

vec3 sampleCubemap(vec3 direction)
{
	/// result.x = U
	/// result.y = V
	/// result.z = index of cubemap texture
	vec3 result = vec3(0.0f);

	// Determine side of the cubemap

	bool positiveX = direction.x > 0.0f;
	bool positiveY = direction.y > 0.0f;
	bool positiveZ = direction.z > 0.0f;

	float absX = abs(direction.x);
	float absY = abs(direction.y);
	float absZ = abs(direction.z);
	float maxAxis = max(absX, absY);
	maxAxis = max(maxAxis, absZ);

	// Stretch the direction vector so that it touches the unit cube
	direction /= maxAxis;

	// Select UV values based on the cubemap side and current shadow cubemap matrices

	if (maxAxis == absX)
	{
		if (positiveX)
		{
			result.x = direction.z;
			result.y = direction.y;
			result.z = 0;
		}
		else
		{
			result.x = -direction.z;
			result.y = direction.y;
			result.z = 1;
		}
	}
	if (maxAxis == absY)
	{
		if (positiveY) 
		{
			result.x = direction.x;
			result.y = direction.z;
			result.z = 2;
		}
		else
		{
			result.x = direction.x;
			result.y = -direction.z;
			result.z = 3;
		}
	}
	if (maxAxis == absZ)
	{
		if (positiveZ) 
		{
			result.x = -direction.x;
			result.y = direction.y;
			result.z = 4;
		}
		else
		{
			result.x = direction.x;
			result.y = direction.y;
			result.z = 5;
		}
	}

	// Convert UV to [0, 1] range

	result.x = 0.5f * result.x + 0.5f;
	result.y = 0.5f * result.y + 0.5f;
	return result;
}

vec3 getPerpendicularVector(vec3 vectorA)
{
    vec3 vectorB = vec3(-1.0f, 0.0f, 0.0f);
    if (abs(dot(vectorB, vectorA)) < 0.000001f)
    {
		vectorB = vec3(0.0f, 0.0f, -1.0f);
    }
    return cross(vectorB, vectorA);
}

float calculatePointShadow(int lightIndex)
{
	float shadowFraction = 0.0f;
	vec3 lightToFragmentDir = positionWorld - pointLights[lightIndex].pos;
	lightToFragmentDir = normalize(lightToFragmentDir);
	
	vec3 samplePos = sampleCubemap(lightToFragmentDir);

	int lightMatrixIndex = int(samplePos.z);
	vec4 fragmentLightSpace = lightMatrices[lightMatrixIndex] * vec4(positionWorld, 1.0f);
	fragmentLightSpace.xyz /= fragmentLightSpace.w;
	if (fragmentLightSpace.z <= 1.0f)
	{
		fragmentLightSpace.xyz = fragmentLightSpace.xyz * 0.5f + 0.5f;
		float currentDepth = fragmentLightSpace.z;

        vec3 perpendicularDir1 = normalize(getPerpendicularVector(lightToFragmentDir));
        vec3 perpendicularDir2 = normalize(cross(lightToFragmentDir, perpendicularDir1));

		float pixelSize = 1.0f / shadowMapTextureSize;
		int radius = 2;
		for (int x = -radius; x <= radius; ++x)
		{
			for (int y = -radius; y <= radius; ++y)
			{
                vec3 perpendicularOffset = perpendicularDir1 * x + perpendicularDir2 * y;
                vec3 sampleDir = lightToFragmentDir + pixelSize * perpendicularOffset;
                samplePos = sampleCubemap(sampleDir);
                float closestDepth = texture(shadowMapPack, samplePos).r;
                if (currentDepth > closestDepth)
                {
                    shadowFraction += 1.0f;
                }
			}
		}
		shadowFraction /= 25.0f;
	}
	return 1.0f - shadowFraction;
}

vec3 calculateLighting(vec3 albedoFragment, float roughness, float metallicity, vec3 norm, float occlusion)
{
	// Ambient light
	vec3 result = vec3(0.0f);
	result = (1.0f - metallicity) * albedoFragment * 0.02f * occlusion;

	// Directional lights
	for (int i = 0; i < amountOfDirectionalLights; i++)
	{
		vec3 lightDir = -directionalLights[i].direction;
		vec3 lightColor = directionalLights[i].color * directionalLights[i].intensity;
		lightDir = normalize(lightDir);
		result += calculateLightSource(albedoFragment, roughness, metallicity, F0, K, norm, dotNormViewDir, lightDir, lightColor);
	}

	// Point lights
	for (int i = 0; i < amountOfPointLights; i++)
	{
		float shadowFraction = calculatePointShadow(i);

		vec3 lightDir = pointLights[i].pos - positionWorld;
		float distance = abs(length(lightDir));
		float attenuation = 0.0f;
		if (pointLights[i].range > 0.0f)
		{
			attenuation = calculateAttenuation(distance, pointLights[i].range);
		}
		vec3 lightColor = pointLights[i].color * pointLights[i].intensity * attenuation * shadowFraction;
		lightDir = normalize(lightDir);
		
		result += calculateLightSource(albedoFragment, roughness, metallicity, F0, K, norm, dotNormViewDir, lightDir, lightColor);
	}

	// Spot lights
	for (int i = 0; i < amountOfSpotLights; i++)
	{
		float shadowFraction = calculateSpotShadow(i);

		vec3 lightDir = spotLights[i].pos - positionWorld;
		float distance = abs(length(lightDir));
		float attenuation = 0.0f;
		if (spotLights[i].range > 0.0f)
		{
			attenuation = calculateAttenuation(distance, spotLights[i].range);
		}
		vec3 lightColor = spotLights[i].color * spotLights[i].intensity * attenuation * shadowFraction;
		lightDir = normalize(lightDir);
	
		float theta = dot(lightDir, normalize(-spotLights[i].direction));
		
		if (theta > cos(spotLights[i].outerConeAngle))
		{
			float epsilon = cos(spotLights[i].innerConeAngle) - cos(spotLights[i].outerConeAngle);
			float spotLightIntensity = clamp((theta - cos(spotLights[i].outerConeAngle)) / epsilon, 0.0f, 1.0f);
			lightColor *= spotLightIntensity;
			result += calculateLightSource(albedoFragment, roughness, metallicity, F0, K, norm, dotNormViewDir, lightDir, lightColor);
		}
	}

	return result;
}

void main()
{
    vec4 positionTextureFragment = texture(positionTexture, Texcoord);
	vec4 normalTextureFragment = texture(normalTexture, Texcoord);
	vec4 albedoTextureFragment = texture(albedoTexture, Texcoord);
	vec4 materialTextureFragment = texture(materialTexture, Texcoord);
    
    if (materialTextureFragment.x < 1.0f)
	{
		discard;
	}
    
	position = positionTextureFragment.rgb;
    positionWorld = vec3(inverseViewMatrix * vec4(position, 1.0f));
	norm = normalTextureFragment.rgb;
	normWorld = normalize(vec3(inverseViewOrientationMatrix * vec4(norm, 1.0f)));
    albedoFragment = vec4(albedoTextureFragment.rgb, 1.0f);
	emissiveFragment = vec3(
        positionTextureFragment.a, 
        normalTextureFragment.a, 
        materialTextureFragment.a);
    metallicity = materialTextureFragment.b;
    roughness = materialTextureFragment.g;
    occlusion = albedoTextureFragment.a;
	
	// Precomputed properties
	viewDir = -position;
	viewDir = normalize(viewDir);
	viewDirWorld = vec3(inverseViewOrientationMatrix * vec4(viewDir, 1.0f));
	roughnessSquared = pow(roughness, 2.0f); // originally ^1.0f
	K = (roughnessSquared + 1.0f) * (roughnessSquared + 1.0f) / 8.0f;
	F0 = vec3(0.04f);
	F0 = mix(F0, albedoFragment.rgb, metallicity);
	dotNormViewDir = max(dot(norm, viewDir), 0.05f); // (Clamping to 0.0f causes artifacts)
	shadowMapTextureSize = textureSize(shadowMapPack, 0).x;
    
	vec4 resultColor = vec4(calculateLighting(albedoFragment.rgb, roughness, metallicity, normWorld, occlusion), 1.0f);
	resultColor += vec4(emissiveFragment, 1.0f);
    
	outColor = resultColor;
}

