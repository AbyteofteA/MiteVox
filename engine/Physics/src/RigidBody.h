
#ifndef PHYSICS_RIGIDBODY_H
#define PHYSICS_RIGIDBODY_H

namespace physcs
{
	struct RigidBody
	{
		float mass = 0;				// 0 - infinity;
		float restitution = 0;		// range: [0; 1];
		float staticFriction = 0;
		float dynamicFriction = 0;
	};
}

#endif