#include "PerlinNoiseGenerator.h"

#include "engine/Math/src/Generators/RandomNumberGenerators/CongruentialGenerator.h"
#include "engine/Math/src/NumericalAnalysis/Intertolation.h"

namespace mathem
{
	float perlinNoise2(float x, float y)
	{
		/*float gradientVector1X = CongruentialGenerator.rand_float(), gradientVector1Y = CongruentialGenerator.rand_float();
		float gradientVector2X = CongruentialGenerator.rand_float(), gradientVector2Y = CongruentialGenerator.rand_float();
		float gradientVector3X = CongruentialGenerator.rand_float(), gradientVector3Y = CongruentialGenerator.rand_float();
		float gradientVector4X = CongruentialGenerator.rand_float(), gradientVector4Y = CongruentialGenerator.rand_float();*/

		float point1 = 0;
		float point2 = 0;
		float point3 = 0;
		float point4 = 0;

		// 

		float tmp1 = interpolate(point1, point2, x / 1.0f);
		float tmp2 = interpolate(point3, point4, x / 1.0f);
		return interpolate(tmp1, tmp2, y / 1.0f);

	}
}