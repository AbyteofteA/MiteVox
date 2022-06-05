#include "Vector.h"

namespace mathem
{
	Vector3D crossProduct(Vector3D vectorA, Vector3D vectorB)
	{
		return { vectorA[1] * vectorB[2] - vectorB[1] * vectorA[2],
				 vectorB[0] * vectorA[2] - vectorA[0] * vectorB[2],
				 vectorA[0] * vectorB[1] - vectorB[0] * vectorA[1] };
	}
}