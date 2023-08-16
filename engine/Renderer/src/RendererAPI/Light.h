#ifndef RENDERER_LIGHT_H
#define RENDERER_LIGHT_H

#include "engine/Renderer/src/RendererAPI/Color.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Quaternion.h"

namespace render
{
    enum struct LightType
    {
        NONE,
        POINT,
        DIRECTIONAL,
        SPOT
    };

    struct LightBase
    {
        ColorRGBf color = { 1.0f, 1.0f, 1.0f };
        float intensity = 1.0f;
        float range = 10.0f;
    };

    struct PointLight
    {
        LightBase lightBase;

        mathem::Vector3D position = { 0.0f, 0.0f, 0.0f };
    };

    struct DirectionalLight
    {
        LightBase lightBase;

        mathem::Vector3D direction = { 0.0f, 0.0f, -1.0f };
    };

    struct SpotLight
    {
        LightBase lightBase;

        mathem::Vector3D position = { 0.0f, 0.0f, 0.0f };
        mathem::Vector3D direction = { 0.0f, 0.0f, -1.0f };
        float innerConeAngle = 0.15f;
        float outerConeAngle = 0.25f;
    };

    union AnyLight
    {
        PointLight pointLight;
        DirectionalLight directionalLight;
        SpotLight spotLight;

        AnyLight();
    };
}

#endif