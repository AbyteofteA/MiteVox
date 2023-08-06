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
        ColorRGBf color;
        float intensity;
        float range;
    };

    struct PointLight
    {
        LightBase lightBase;

        mathem::Vector3D position;
    };

    struct DirectionalLight
    {
        LightBase lightBase;

        mathem::Quaternion direction;
    };

    struct SpotLight
    {
        LightBase lightBase;

        mathem::Vector3D position;
        mathem::Quaternion direction;
        float innerConeAngle = 0.0f;
        float outerConeAngle = 45.0f;
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