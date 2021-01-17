
#ifndef RENDERER_LIGHT_H
#define RENDERER_LIGHT_H

namespace render
{
    struct AmbientLight
    {
        ColorRGBAf color;
    };

    struct DirectedLight
    {
        mathem::Vector3D direction;
        ColorRGBAf color;
    };

    struct PointLight
    {
        mathem::Point3D position;
        ColorRGBAf color;

        float constant;
        float linear;
        float quadratic;
    };

    struct SpotLight
    {
        mathem::Point3D position;
        mathem::Vector3D direction;
        float angle;
        ColorRGBAf color;

        float constant;
        float linear;
        float quadratic;
    };
}

#endif