
#ifndef PHYSICS_INTERSECTAABB_H
#define PHYSICS_INTERSECTAABB_H

#include "engine/Physics/src/Colliders/Primitives.h"
#include "engine/Physics/src/Colliders/PrimitiveCollider.h"
#include "engine/Math/src/LinearAlgebra/Point3D.h"

namespace physcs
{
	// TODO: calculate collisPoint.
	inline bool intersectAABB(Block block1, Block block2, mathem::Point3D* collisPoint)
	{
		float maxX = block1.halfSize.x + block2.halfSize.x;
		float maxY = block1.halfSize.y + block2.halfSize.y;
		float maxZ = block1.halfSize.z + block2.halfSize.z;

		if (std::abs(block1.pos.x - block2.pos.x) > maxX)
		{
			return false;
		}
		if (std::abs(block1.pos.y - block2.pos.y) > maxY)
		{
			return false;
		}
		if (std::abs(block1.pos.z - block2.pos.z) > maxZ)
		{
			return false;
		}

		return true;
	}
	inline bool intersectAABB(Block block, Sphere sphere, mathem::Point3D* collisPoint)
	{
		collisPoint->x =
			fmax(block.pos.x - block.halfSize.x,
				fmin(sphere.pos.x, block.pos.x + block.halfSize.x));
		collisPoint->y =
			fmax(block.pos.y - block.halfSize.y,
				fmin(sphere.pos.y, block.pos.y + block.halfSize.y));
		collisPoint->z =
			fmax(block.pos.z - block.halfSize.z,
				fmin(sphere.pos.z, block.pos.z + block.halfSize.z));

		mathem::Point3D distance;
		distance.x = collisPoint->x - sphere.radius;
		distance.y = collisPoint->y - sphere.radius;
		distance.z = collisPoint->z - sphere.radius;

		if (distance.x * distance.x +
			distance.y * distance.y +
			distance.z * distance.z > sphere.radius * sphere.radius)
		{
			return false;
		}

		return true;
	}
	inline bool intersectAABB(Block block, Cylinder cylinder, mathem::Point3D* collisPoint)
	{
		float maxY = block.halfSize.y + cylinder.halfHeight;
		collisPoint->x =
			fmax(block.pos.x - block.halfSize.x,
				fmin(cylinder.pos.x, block.pos.x + block.halfSize.x));
		collisPoint->y =
			fmax(block.pos.y - block.halfSize.y,
				fmin(cylinder.pos.y, block.pos.y + block.halfSize.y));
		collisPoint->z =
			fmax(block.pos.z - block.halfSize.z,
				fmin(cylinder.pos.z, block.pos.z + block.halfSize.z));

		mathem::Point3D distance;
		distance.x = collisPoint->x - cylinder.radius;
		distance.z = collisPoint->z - cylinder.radius;

		if (distance.x * distance.x +
			distance.z * distance.z > cylinder.radius * cylinder.radius)
		{
			return false;
		}
		if (std::abs(block.pos.y - cylinder.pos.y) > maxY)
		{
			return false;
		}

		return true;
	}

	// TODO: calculate collisPoint.
	inline bool intersectAABB(Sphere sphere1, Sphere sphere2, mathem::Point3D* collisPoint)
	{
		float maxR = sphere1.radius + sphere2.radius;

		if (std::abs(sphere1.pos.x - sphere2.pos.x) > maxR)
		{
			return false;
		}
		if (std::abs(sphere1.pos.y - sphere2.pos.y) > maxR)
		{
			return false;
		}
		if (std::abs(sphere1.pos.z - sphere2.pos.z) > maxR)
		{
			return false;
		}

		return true;
	}
	// TODO: calculate collisPoint.
	inline bool intersectAABB(Sphere sphere, Cylinder cylinder, mathem::Point3D* collisPoint)
	{
		float maxY = sphere.radius + cylinder.halfHeight;
		float maxR = sphere.radius + cylinder.radius;

		if (std::abs(sphere.pos.y - cylinder.pos.y) > maxY)
		{
			return false;
		}
		if (std::abs(sphere.pos.x - cylinder.pos.x) > maxR)
		{
			return false;
		}
		if (std::abs(sphere.pos.z - cylinder.pos.z) > maxR)
		{
			return false;
		}

		return true;
	}

	// TODO: calculate collisPoint.
	inline bool intersectAABB(Cylinder cylinder1, Cylinder cylinder2, mathem::Point3D* collisPoint)
	{
		float maxY = cylinder1.halfHeight + cylinder2.halfHeight;
		float maxXZ = cylinder1.radius + cylinder2.radius;

		if (std::abs(cylinder1.pos.x - cylinder2.pos.x) > maxXZ)
		{
			return false;
		}
		if (std::abs(cylinder1.pos.y - cylinder2.pos.y) > maxY)
		{
			return false;
		}
		if (std::abs(cylinder1.pos.z - cylinder2.pos.z) > maxXZ)
		{
			return false;
		}

		return true;
	}

	inline bool intersectAABB(PrimitiveCollider collider1, PrimitiveCollider collider2, mathem::Point3D* collisPoint)
	{
		unsigned int amountOfBlocks1 = collider1.blocks.getSizeData();
		unsigned int amountOfSpheres1 = collider1.spheres.getSizeData();
		unsigned int amountOfCylinders1 = collider1.cylinders.getSizeData();
		
		unsigned int amountOfBlocks2 = collider2.blocks.getSizeData();
		unsigned int amountOfSpheres2 = collider2.spheres.getSizeData();
		unsigned int amountOfCylinders2 = collider2.cylinders.getSizeData();
		
		for (unsigned int i = 0; i < amountOfBlocks1; i++)
		{
			for (unsigned int j = 0; j < amountOfBlocks2; j++)
			{
				if (intersectAABB(collider1.blocks.data[i], collider2.blocks.data[j], collisPoint))
				{
					return true;
				}
			}
			for (unsigned int j = 0; j < amountOfSpheres2; j++)
			{
				if (intersectAABB(collider1.blocks.data[i], collider2.spheres.data[j], collisPoint))
				{
					return true;
				}
			}
			for (unsigned int j = 0; j < amountOfCylinders2; j++)
			{
				if (intersectAABB(collider1.blocks.data[i], collider2.cylinders.data[j], collisPoint))
				{
					return true;
				}
			}
		}
		for (unsigned int i = 0; i < amountOfSpheres1; i++)
		{
			for (unsigned int j = 0; j < amountOfSpheres2; j++)
			{
				if (intersectAABB(collider1.spheres.data[i], collider2.spheres.data[j], collisPoint))
				{
					return true;
				}
			}
			for (unsigned int j = 0; j < amountOfCylinders2; j++)
			{
				if (intersectAABB(collider1.spheres.data[i], collider2.cylinders.data[j], collisPoint))
				{
					return true;
				}
			}
		}
		for (unsigned int i = 0; i < amountOfCylinders1; i++)
		{
			for (unsigned int j = 0; j < amountOfCylinders2; j++)
			{
				if (intersectAABB(collider1.cylinders.data[i], collider2.cylinders.data[j], collisPoint))
				{
					return true;
				}
			}
		}

		return false;
	}
}



#endif
