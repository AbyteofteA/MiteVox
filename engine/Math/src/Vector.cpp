#include "Vector.h"

namespace mathem
{
	Vector3D crossProduct(Vector3D vectorA, Vector3D vectorB)
	{
		return { vectorA.y() * vectorB.z() - vectorB.y() * vectorA.z(),
				 vectorB.x() * vectorA.z() - vectorA.x() * vectorB.z(),
				 vectorA.x() * vectorB.y() - vectorB.x() * vectorA.y() };
	}
}