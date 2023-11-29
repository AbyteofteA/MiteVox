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

uniform float deltaTime = 0.0f;
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
int shadowSamplesCount = 20;

// Precomputed properties

float noiseFromScreenUV = 0.0f;
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
float shadowMapPixelSize = 0.0f;

#define PI 3.1415926535897932384626433832795
#define GOLDEN_ANGLE 2.399967f
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

float convertToLinearDepth(float depthFragment, float near, float far)
{
	depthFragment = 2.0f * depthFragment - 1.0f;
	return 2.0f * near * far / 
        (far + near - depthFragment * (far - near));
}

float projectSpotShadowTexelSize(vec3 lightToFragment, vec3 lightDirection, float lightOuterConeAngle)
{
    float positionProjection = abs(dot(lightToFragment, lightDirection));
    return 2.0f * positionProjection * tan(lightOuterConeAngle) * shadowMapPixelSize;
}

vec2 vogelDisk(int sampleIndex, int samplesCount, float angularOffset)
{
    float radius = sqrt(sampleIndex + 0.5f) / sqrt(samplesCount);
    float angle = sampleIndex * GOLDEN_ANGLE + angularOffset;
    return vec2(radius * cos(angle), radius * sin(angle));
}

float calculateSpotShadow(int lightIndex, vec3 lightDir)
{
    // Calculate shadow map texel size from original fragment position
    vec3 lightToFragment = positionWorld - spotLights[lightIndex].pos;
    vec3 lightToFragmentDir = normalize(lightToFragment);
    float shadowMapTexelSize = projectSpotShadowTexelSize(
        lightToFragment, lightDir, spotLights[lightIndex].outerConeAngle);
    
    // Calculate offsets
    float normalDotLightDirection = 1.0f - dot(normWorld, -lightToFragmentDir);
    float depthOffset = max(10.0f * normalDotLightDirection, 8.0f) * shadowMapTexelSize;
    float normalOffset = max(8.0f * normalDotLightDirection, 2.0f) * shadowMapTexelSize;
    
	float shadowFraction = 0.0f;
	vec4 fragmentLightSpace = lightMatrices[lightIndex] * vec4(positionWorld, 1.0f);
	fragmentLightSpace.xyz /= fragmentLightSpace.w;
    fragmentLightSpace.xyz = fragmentLightSpace.xyz * 0.5f + 0.5f;
    float fragmentDepthFromLightsView = convertToLinearDepth(fragmentLightSpace.z, 0.1f, spotLights[lightIndex].range);
    
    // Calculate sample position based on the fragment position shifted by normal
    vec3 modifiedPositionWorld = positionWorld + normalOffset * normWorld;
	vec4 modifiedFragmentLightSpace = lightMatrices[lightIndex] * vec4(modifiedPositionWorld, 1.0f);
	modifiedFragmentLightSpace.xyz /= modifiedFragmentLightSpace.w;
    modifiedFragmentLightSpace.xyz = modifiedFragmentLightSpace.xyz * 0.5f + 0.5f;
    
    for (int i = 0; i < shadowSamplesCount; ++i)
    {
        vec2 sampleOffset = vogelDisk(i, shadowSamplesCount, noiseFromScreenUV * 2.0f * PI);
        vec3 samplePos;
        samplePos.xy = modifiedFragmentLightSpace.xy + sampleOffset * 8.0f * shadowMapPixelSize;
        samplePos.z = lightIndex;
        
        float closestDepth = texture(shadowMapPack, samplePos).r;
        closestDepth = convertToLinearDepth(closestDepth, 0.1f, spotLights[lightIndex].range);
        
        shadowFraction += (fragmentDepthFromLightsView - depthOffset) > closestDepth ? 1.0f : 0.0f;
    }
    shadowFraction /= shadowSamplesCount;
    shadowFraction *= fragmentLightSpace.z <= 1.0f ? 1.0f : 0.0f;
	return 1.0f - shadowFraction;
}

vec3 sampleCubemap(vec3 direction)
{
	/// result.x = U
	/// result.y = V
	/// result.z = index of cubemap texture
	vec3 result = vec3(0.0f);

	// Determine side of the cubemap

	float signAxisX = direction.x > 0.0f ? 1.0f : -1.0f;
	float signAxisY = direction.y > 0.0f ? 1.0f : -1.0f;
	float signAxisZ = direction.z > 0.0f ? 1.0f : -1.0f;

	vec3 absDirection = abs(direction);
	float maxAxis = max(absDirection.x, absDirection.y);
	maxAxis = max(maxAxis, absDirection.z);

	// Stretch the direction vector so that it touches the unit cube
	direction /= maxAxis;

	// Select UV values based on the cubemap side and current shadow cubemap matrices

	if (maxAxis == absDirection.x)
	{
		result = vec3(
            direction.z * signAxisX,
            direction.y,
            max(-signAxisX, 0.0f));
	}
	if (maxAxis == absDirection.y)
	{
        result = vec3(
            direction.x,
            direction.z * signAxisY,
            2.0f + max(-signAxisY, 0.0f));
	}
	if (maxAxis == absDirection.z)
	{
        result = vec3(
            -direction.x * signAxisZ,
            direction.y, 
            4.0f + max(-signAxisZ, 0.0f));
	}

	// Convert UV to [0, 1] range
	result.xy = 0.5f * result.xy + 0.5f;
    
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

const vec3 cubemapDirections[6] = vec3[6]
(
    vec3(1.0f, 0.0f, 0.0f),
    vec3(-1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, -1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f),
    vec3(0.0f, 0.0f, -1.0f)
);

const mat3 cubemapSidesOrientations[6] = mat3[6]
(
    mat3(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)),
    mat3(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f)),
    
    mat3(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)),
    mat3(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)),
    
    mat3(vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
    mat3(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f))
);

float projectPointShadowTexelSize(vec3 lightToFragment, int cubemapSide)
{
    float positionProjection = abs(dot(lightToFragment, cubemapDirections[cubemapSide]));
    return 2.0f * positionProjection * shadowMapPixelSize;
}

float calculatePointShadow(int lightIndex)
{
	float shadowFraction = 0.0f;
	
	// Calculate shadow map texel size from original fragment position
    vec3 lightToFragment = positionWorld - pointLights[lightIndex].pos;
	vec3 lightToFragmentDir = normalize(lightToFragment);
    vec3 samplePos = sampleCubemap(lightToFragmentDir);
	int lightMatrixIndex = int(samplePos.z);
    float shadowMapTexelSize = projectPointShadowTexelSize(lightToFragment, lightMatrixIndex);
    
    // Calculate offsets
    float normalDotLightDirection = 1.0f - dot(normWorld, -lightToFragmentDir);
    float depthOffset = max(10.0f * normalDotLightDirection, 8.0f) * shadowMapTexelSize;
    float normalOffset = max(8.0f * normalDotLightDirection, 0.0f) * shadowMapTexelSize;
    
    // Calculate sample position based on the fragment position shifted by normal
    vec3 modifiedPositionWorld = positionWorld + normalOffset * normWorld;
	lightToFragment = modifiedPositionWorld - pointLights[lightIndex].pos;
	lightToFragmentDir = normalize(lightToFragment);
	samplePos = sampleCubemap(lightToFragmentDir);
	lightMatrixIndex = int(samplePos.z);
    
	vec4 fragmentLightSpace = lightMatrices[lightMatrixIndex] * vec4(positionWorld, 1.0f);
	fragmentLightSpace.xyz /= fragmentLightSpace.w;
    fragmentLightSpace.xyz = fragmentLightSpace.xyz * 0.5f + 0.5f;
    float fragmentDepthFromLightsView = convertToLinearDepth(fragmentLightSpace.z, 0.1f, pointLights[lightIndex].range);

    for (int i = 0; i < shadowSamplesCount; ++i)
    {
        vec2 sampleOffset = vogelDisk(i, shadowSamplesCount, noiseFromScreenUV * 2.0f * PI);
        sampleOffset *= (16.0f * shadowMapPixelSize);
        sampleOffset = 2.0f * samplePos.xy - 1.0f + sampleOffset;
        vec3 sampleDir = cubemapSidesOrientations[lightMatrixIndex] * vec3(sampleOffset, 1.0f);
        vec3 currentSamplePos = sampleCubemap(sampleDir);
        
        float closestDepth = convertToLinearDepth(texture(shadowMapPack, currentSamplePos).r, 0.1f, pointLights[lightIndex].range);
        shadowFraction += (fragmentDepthFromLightsView - depthOffset) > closestDepth ? 1.0f : 0.0f;
    }
    shadowFraction /= shadowSamplesCount;
    shadowFraction *= fragmentLightSpace.z <= 1.0f ? 1.0f : 0.0f;
	return 1.0f - shadowFraction;
}

vec3 calculateLighting(vec3 albedoFragment, float roughness, float metallicity, vec3 norm, float occlusion)
{
	// Ambient light
	vec3 result = vec3(0.0f);
	result = (1.0f - metallicity) * albedoFragment * 0.025f * occlusion;

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
        vec3 lightDir = normalize(spotLights[i].direction);
		float shadowFraction = calculateSpotShadow(i, lightDir);

		vec3 fragmentToLightDir = spotLights[i].pos - positionWorld;
		float distance = abs(length(fragmentToLightDir));
		float attenuation = 0.0f;
		if (spotLights[i].range > 0.0f)
		{
			attenuation = calculateAttenuation(distance, spotLights[i].range);
		}
		vec3 lightColor = spotLights[i].color * spotLights[i].intensity * attenuation * shadowFraction;
		fragmentToLightDir = normalize(fragmentToLightDir);
	
		float theta = dot(fragmentToLightDir, -lightDir);
		
		if (theta > cos(spotLights[i].outerConeAngle))
		{
			float epsilon = cos(spotLights[i].innerConeAngle) - cos(spotLights[i].outerConeAngle);
			float spotLightIntensity = clamp((theta - cos(spotLights[i].outerConeAngle)) / epsilon, 0.0f, 1.0f);
			lightColor *= spotLightIntensity;
			result += calculateLightSource(albedoFragment, roughness, metallicity, F0, K, norm, dotNormViewDir, fragmentToLightDir, lightColor);
		}
	}

	return result;
}

float noise(vec2 screenUV)
{
	screenUV += vec2(1.0f - deltaTime, deltaTime);
	return fract(12345.06789f * sin(dot(screenUV.xy, vec2(123.45f, 678.90f))));
}

void main()
{
    noiseFromScreenUV = noise(Texcoord);
    
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
	shadowMapPixelSize = 1.0f / textureSize(shadowMapPack, 0).x;
    
	vec4 resultColor = vec4(calculateLighting(albedoFragment.rgb, roughness, metallicity, normWorld, occlusion), 1.0f);
	resultColor += vec4(emissiveFragment, 1.0f);
    
	outColor = resultColor;
}

