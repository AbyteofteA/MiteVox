#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionType.h"

#include <cstdint>
#include <limits>

#define MAX_CONTACT_POINTS_COUNT 4

namespace mathem
{
	struct CollisionProperties
	{
		CollisionType type = CollisionType::NONE;
		float penetrationDepth = std::numeric_limits<float>::max();
		bool normalBelongsToTheFirst = true;
		Vector3D normal;
		uint16_t contactPointsCount = 0;
		Vector3D contactPoints[MAX_CONTACT_POINTS_COUNT];
		Vector3D forces[MAX_CONTACT_POINTS_COUNT];
		float contactPointsDistancesSquared[MAX_CONTACT_POINTS_COUNT] = { 
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max() };

		void invert();
		void recomputePenetrationAndNormal(float penetrationDepth, Vector3D normal, bool normalBelongsToTheFirst);
		void resetContactPoints();
		void tryAddNewContactPoint(Vector3D contactPoint, float distancesSquared, float equalityTolerance);
		void concatenate(CollisionProperties& other, float equalityTolerance);
	};

	template <class T>
	struct CollisionInfo
	{
		CollisionProperties properties;

		T object1 = nullptr;
		T object2 = nullptr;
	};
}

#endif