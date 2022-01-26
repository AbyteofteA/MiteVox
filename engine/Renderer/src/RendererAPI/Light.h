
#ifndef RENDERER_LIGHT_H
#define RENDERER_LIGHT_H

namespace render
{
    struct DirectedLight
    {
        mathem::Vector3D direction;
        ColorRGBf color;

        float intensity;
        float range;
    };

    struct PointLight
    {
        mathem::Point3D position;
        ColorRGBf color;

        float constant;
        float linear;
        float quadratic;

        float intensity;
        float range;
    };

    struct SpotLight
    {
        mathem::Point3D position;
        mathem::Vector3D direction;
        float angle;
        ColorRGBf color;

        float constant;
        float linear;
        float quadratic;

        float intensity;
        float range;
    };
}

#endif