
#ifndef PHYSICS_KINEMATICS_H
#define PHYSICS_KINEMATICS_H

#include "Physics/src/MovementProps.h"

namespace physcs
{
	void applyKinematics(MovementProps* body, float time)
	{
		body->velocity.i += body->acceler.i * time;
		body->velocity.j -= body->acceler.j * time;
		body->velocity.k += body->acceler.k * time;

		body->pos.i += body->velocity.i * time;
		body->pos.j -= body->velocity.j * time;
		body->pos.k += body->velocity.k * time;

		body->angularVelocity.i += body->angularAcceler.i * time;
		body->angularVelocity.j += body->angularAcceler.j * time;
		body->angularVelocity.k += body->angularAcceler.k * time;

		body->angle.i += body->angularVelocity.i * time;
		body->angle.j += body->angularVelocity.j * time;
		body->angle.k += body->angularVelocity.k * time;

		body->acceler.i = 0;
		body->acceler.j = 0;
		body->acceler.k = 0;
		body->angularAcceler.i = 0;
		body->angularAcceler.j = 0;
		body->angularAcceler.k = 0;
	}
}


#endif