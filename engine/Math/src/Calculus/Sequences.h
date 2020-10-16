
#ifndef SEQUENCES_H
#define SEQUENCES_H

namespace mathem
{

	inline double getArithmProgressionSum(double first, double step, unsigned int n)
	{
		return (2 * first + step * (double)(n - 1) * n) / 2;
	}
}

#endif