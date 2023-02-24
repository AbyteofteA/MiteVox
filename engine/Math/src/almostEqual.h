#ifndef ALMOSTEQUAL_H
#define ALMOSTEQUAL_H

#include "engine/Math/src/Vector.h"

#include <algorithm>

namespace mathem
{
	template <class T>
	inline bool almostEqual(T value1, T value2, T equalityTolerance);

	template <class T, size_t Dimension>
	inline bool almostEqual(Vector<T, Dimension> value1, Vector<T, Dimension> value2, T equalityTolerance);

	template <class T>
	inline bool almostEqual(T value1, T value2, T equalityTolerance)
	{
		if (std::abs(value1 - value2) > equalityTolerance)
		{
			return false;
		}
		return true;
	}

	template <class T, size_t Dimension>
	inline bool almostEqual(Vector<T, Dimension> vector1, Vector<T, Dimension> vector2, T equalityTolerance)
	{
		for (size_t i = 0; i < Dimension; ++i)
		{
			if (almostEqual<T>(vector1.data[i], vector2.data[i], equalityTolerance) == false)
			{
				return false;
			}
		}
		return true;
	}
}

#endif