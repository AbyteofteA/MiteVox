#ifndef COMPUTEPENETRATION_H
#define COMPUTEPENETRATION_H

namespace mathem
{
	/// <summary>
	/// Tells how much the object1 must be moved to eliminate the intersection, if any.
	/// </summary>
	/// <param name="object1ProjectionMin"></param>
	/// <param name="object1ProjectionMax"></param>
	/// <param name="object2ProjectionMin"></param>
	/// <param name="object2ProjectionMax"></param>
	/// <returns></returns>
	float computePenetration(float object1ProjectionMin, float object1ProjectionMax, float object2ProjectionMin, float object2ProjectionMax);
}

#endif