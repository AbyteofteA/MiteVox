#include "MovementProperties.h"

namespace physcs
{
	float MovementProperties::getMass()
	{
		if (inverseMass == 0.0f)
		{
			return 0.0f;
		}
		return 1.0f / inverseMass;
	}
}