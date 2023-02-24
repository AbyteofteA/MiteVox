#include "computePenetration.h"

#include <algorithm>

namespace mathem
{
	float computePenetration(float object1ProjectionMin, float object1ProjectionMax, float object2ProjectionMin, float object2ProjectionMax)
	{
		if (object1ProjectionMax <= object2ProjectionMin ||
			object2ProjectionMax <= object1ProjectionMin)
		{
			return 0.0f;
		}

		float penetration = -1.0f;
		if (object1ProjectionMin > object2ProjectionMin)
		{
			penetration = 1.0f;
		}

		float delta1 = std::abs(object1ProjectionMax - object2ProjectionMin);
		float delta2 = std::abs(object2ProjectionMax - object1ProjectionMin);
		if (delta1 < delta2)
		{
			penetration *= delta1;
		}
		else
		{
			penetration *= delta2;
		}
		return penetration;
	}
}