#include "CollisionInfo.h"

#include "engine/Math/src/almostEqual.h"
#include "engine/Math/src/MinAndMax.h"
#include <algorithm>

namespace mathem
{
	void CollisionProperties::reset()
	{
		type = CollisionType::NONE;
		penetrationDepth = mathem::max<float>();

		resetContactPoints();
	}

	void CollisionProperties::invert()
	{
		penetrationDepth = -penetrationDepth;
		normal = -normal;
	}

	void CollisionProperties::recomputePenetrationAndNormal(float penetrationDepth, Vector3D normal, bool normalBelongsToTheFirst)
	{
		if (std::abs(penetrationDepth) < std::abs(this->penetrationDepth))
		{
			this->normalBelongsToTheFirst = normalBelongsToTheFirst;
			this->penetrationDepth = penetrationDepth;
			this->normal = normal;

			// Make sure that the normal looks towards the second geometry
			if (penetrationDepth < 0.0f)
			{
				invert();
			}
		}
	}

	void CollisionProperties::resetContactPoints()
	{
		contactPointsCount = 0;
		for (size_t i = 0; i < MAX_CONTACT_POINTS_COUNT; ++i)
		{
			contactPointsDistancesSquared[i] = mathem::max<float>();
		}
	}

	void CollisionProperties::tryAddNewContactPoint(Vector3D contactPoint, float distanceSquared, float equalityTolerance)
	{
		int32_t contactToChangeIndex = -1;
		float contactToChangeDistance = mathem::max<float>();
		for (size_t i = 0; i < contactPointsCount; ++i)
		{
			if (almostEqual(contactPoints[i], contactPoint, equalityTolerance))
			{
				return;
			}
			if (distanceSquared < contactPointsDistancesSquared[i])
			{
				if (contactToChangeIndex != -1 &&
					contactToChangeDistance > contactPointsDistancesSquared[i])
				{
					continue;
				}
				contactToChangeIndex = i;
				contactToChangeDistance = contactPointsDistancesSquared[i];
			}
		}
		if (contactPointsCount < MAX_CONTACT_POINTS_COUNT)
		{
			contactToChangeIndex = contactPointsCount;
			contactPointsCount += 1;
		}
		if (contactToChangeIndex == -1)
		{
			return;
		}
		contactPoints[contactToChangeIndex] = contactPoint;
		contactPointsDistancesSquared[contactToChangeIndex] = distanceSquared;
	}

	void CollisionProperties::concatenate(CollisionProperties& other, float equalityTolerance)
	{
		switch (type)
		{
		case mathem::CollisionType::NONE:
			type = other.type;
			break;

		case mathem::CollisionType::INSCRIBTION_1_2:
		case mathem::CollisionType::INSCRIBTION_2_1:
			if (type != other.type)
			{
				type = mathem::CollisionType::INTERSECTION;
			}
			break;

		case mathem::CollisionType::INTERSECTION:
		default:
			break;
		}

		recomputePenetrationAndNormal(other.penetrationDepth, other.normal, other.normalBelongsToTheFirst);

		for (size_t i = 0; i < other.contactPointsCount; ++i)
		{
			tryAddNewContactPoint(other.contactPoints[i], other.contactPointsDistancesSquared[i], equalityTolerance);
		}
	}
}