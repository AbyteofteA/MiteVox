
#ifndef PHYSICS_DYNAMICS_H
#define PHYSICS_DYNAMICS_H

namespace physcs
{
	inline void applyLawsOfConservation(
		MovementProps* props1, MovementProps* props2,
		RigidBody* body1, RigidBody* body2)
	{
		/*float netEnergyX = pow(body1->velocityX, 2) * body1->mass + pow(body2->velocityX, 2) * body2->mass;
		float netEnergyY = pow(body1->velocityY, 2) * body1->mass + pow(body2->velocityY, 2) * body2->mass;
		float netEnergyZ = pow(body1->velocityZ, 2) * body1->mass + pow(body2->velocityZ, 2) * body2->mass;*/

		float netImpulseX = props1->velocity.x() * body1->mass + props2->velocity.x() * body2->mass;
		float netImpulseY = props1->velocity.y() * body1->mass + props2->velocity.y() * body2->mass;
		float netImpulseZ = props1->velocity.z() * body1->mass + props2->velocity.z() * body2->mass;
		float netMass = body1->mass + body2->mass;

		float vX = (2 * body2->mass * props2->velocity.x() + (body1->mass - body2->mass) * props1->velocity.x()) / netMass;
		float vY = (2 * body2->mass * props2->velocity.y() + (body1->mass - body2->mass) * props1->velocity.y()) / netMass;
		float vZ = (2 * body2->mass * props2->velocity.z() + (body1->mass - body2->mass) * props1->velocity.z()) / netMass;
		props1->velocity.x() = vX;
		props1->velocity.y() = vY;
		props1->velocity.z() = vZ;

		/*float sign = 1;
		if (body2->velocityX <= 0)
			sign = 1;
		else
			sign = -1;
		float aX = pow(body2->mass, 2) / body1->mass + body2->mass;
		float bX = -netImpulseX * body2->mass / body1->mass;
		float cX = pow(netImpulseX, 2) / body1->mass - netImpulseX;
		float DX = pow(bX, 2) - 4 * aX * cX;
		float vX = (-bX + sign * sqrt(abs(DX))) / (2 * aX);
		body2->velocityX = vX;

		if (body2->velocityY <= 0)
			sign = 1;
		else
			sign = -1;
		float aY = pow(body2->mass, 2) / body1->mass + body2->mass;
		float bY = -netImpulseY * body2->mass / body1->mass;
		float cY = pow(netImpulseY, 2) / body1->mass - netImpulseY;
		float DY = pow(bY, 2) - 4 * aY * cY;
		float vY = (-bY + sign * sqrt(abs(DY))) / (2 * aY);
		body2->velocityY = vY;

		if (body2->velocityZ <= 0)
			sign = 1;
		else
			sign = -1;
		float aZ = pow(body2->mass, 2) / body1->mass + body2->mass;
		float bZ = -netImpulseZ * body2->mass / body1->mass;
		float cZ = pow(netImpulseZ, 2) / body1->mass - netImpulseZ;
		float DZ = pow(bZ, 2) - 4 * aZ * cZ;
		float vZ = (-bZ + sign * sqrt(abs(DZ))) / (2 * aZ);
		body2->velocityZ = vZ;*/




		/*float sign = 1;
		if (body2->velocityX <= 0)
			sign = 1;
		else
			sign = -1;
		float DX = body2->mass * (-3 * body2->mass / pow(body1->mass, 2) * pow(netImpulseX, 2) +
					4 * (netEnergyX * (body2->mass / body1->mass + 1) - pow(netImpulseX, 2) / body1->mass));
		float vX = (netImpulseX * body2->mass / body1->mass + sign * sqrt(abs(DX))) /
							2 * (pow(body2->mass, 2) / body1->mass + body2->mass);
		body2->velocityX += vX;

		if (body2->velocityY <= 0)
			sign = 1;
		else
			sign = -1;
		float DY = body2->mass * (-3 * body2->mass / pow(body1->mass, 2) * pow(netImpulseY, 2) +
					4 * (netEnergyY * (body2->mass / body1->mass + 1) - pow(netImpulseY, 2) / body1->mass));
		float vY = (netImpulseY * body2->mass / body1->mass + sign * sqrt(abs(DY))) /
							2 * (pow(body2->mass, 2) / body1->mass + body2->mass);
		body2->velocityY += vY;

		if (body2->velocityZ <= 0)
			sign = 1;
		else
			sign = -1;
		float DZ = body2->mass * (-3 * body2->mass / pow(body1->mass, 2) * pow(netImpulseZ, 2) +
					4 * (netEnergyZ * (body2->mass / body1->mass + 1) - pow(netImpulseZ, 2) / body1->mass));
		float vZ = (netImpulseZ * body2->mass / body1->mass + sign * sqrt(abs(DZ))) /
							2 * (pow(body2->mass, 2) / body1->mass + body2->mass);
		body2->velocityZ += vZ;*/

		props2->velocity.x() = (netImpulseX - vX * body1->mass) / body2->mass;
		props2->velocity.y() = (netImpulseY - vY * body1->mass) / body2->mass;
		props2->velocity.z() = (netImpulseZ - vZ * body1->mass) / body2->mass;
	}
}


#endif