
#ifndef PHYSICS_KINEMATICS_H
#define PHYSICS_KINEMATICS_H

#include "engine/Physics/src/MovementProps.h"

namespace physcs
{
	inline void applyKinematics(MovementProps* body, float time)
	{
		body->velocity.x() += body->acceler.x() * time;
		body->velocity.y() -= body->acceler.y() * time;
		body->velocity.z() += body->acceler.z() * time;

		body->pos.x() += body->velocity.x() * time;
		body->pos.y() -= body->velocity.y() * time;
		body->pos.z() += body->velocity.z() * time;

		body->angularVelocity.x() += body->angularAcceler.x() * time;
		body->angularVelocity.y() += body->angularAcceler.y() * time;
		body->angularVelocity.z() += body->angularAcceler.z() * time;

		body->angle.x() += body->angularVelocity.x() * time;
		body->angle.y() += body->angularVelocity.y() * time;
		body->angle.z() += body->angularVelocity.z() * time;

		body->acceler.x() = 0;
		body->acceler.y() = 0;
		body->acceler.z() = 0;
		body->angularAcceler.x() = 0;
		body->angularAcceler.y() = 0;
		body->angularAcceler.z() = 0;
	}
}


#endif