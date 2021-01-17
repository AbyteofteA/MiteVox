
#ifndef PHYSICS_PRIMITIVES_H
#define PHYSICS_PRIMITIVES_H

#include "Math/src/Math.h"

namespace physcs
{
	enum struct PrimitiveType { NONE = 0, PARALLELEPIPED, SPHERE, CYLINDER };

	struct Block
	{
		mathem::Point3D pos = { 0.f };
		mathem::Point3D rotation = { 0.f };
		mathem::Point3D halfSize = { 0.5f, 0.5f, 0.5f };
	};

	struct Sphere
	{
		mathem::Point3D pos = { 0.f };
		mathem::Point3D rotation = { 0.f };
		float radius = 0.5f;
	};

	struct Cylinder
	{
		mathem::Point3D pos = { 0.f };
		mathem::Point3D rotation = { 0.f };
		float halfHeight = 0.5f;
		float radius = 0.5f;
	};
}

#endif