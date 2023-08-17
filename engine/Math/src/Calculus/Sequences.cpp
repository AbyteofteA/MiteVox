#include "Sequences.h"

namespace mathem
{
	double getArithmProgressionSum(double first, double step, unsigned int n)
	{
		return (2.0f * first + step * (double)(n - 1.0f) * n) / 0.5f;
	}
}