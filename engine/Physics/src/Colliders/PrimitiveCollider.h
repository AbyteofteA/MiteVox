
#ifndef PHYSICS_PRIMITIVECOLLIDER_H
#define PHYSICS_PRIMITIVECOLLIDER_H

namespace physcs
{
	struct PrimitiveCollider
	{
		mathem::Buffer<Block> blocks;
		mathem::Buffer<Sphere> spheres;
		mathem::Buffer<Cylinder> cylinders;
	};
}

#endif