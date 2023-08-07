#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/MinAndMax.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionType.h"

#include <cstdint>

#define MAX_CONTACT_POINTS_COUNT 4

namespace mathem
{
	/// <summary>
	/// Stores a contact point information between object1 and object2
	/// </summary>
	struct CollisionContact
	{
		Vector3D contactPoints1; // In local coordinates of object1
		Vector3D contactPoints2; // In local coordinates of object2
		float normalForce = 0.0f;
		float tangentForce = 0.0f;
	};

	struct CollisionProperties
	{
		CollisionType type = CollisionType::NONE;
		float penetrationDepth = mathem::max<float>();
		bool normalBelongsToTheFirst = true;
		Vector3D normal;
		uint16_t contactPointsCount = 0;
		Vector3D contactPoints[MAX_CONTACT_POINTS_COUNT];
		CollisionContact contacts[MAX_CONTACT_POINTS_COUNT];
		float contactPointsDistancesSquared[MAX_CONTACT_POINTS_COUNT] = { 
			mathem::max<float>(),
			mathem::max<float>(),
			mathem::max<float>(),
			mathem::max<float>() };

		void reset();
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

		inline void reset();
	};

	template <class T>
	inline void CollisionInfo<T>::reset()
	{
		properties.reset();
		object1 = nullptr;
		object2 = nullptr;
	}
}

#endif