#ifndef MINANDMAX_H
#define MINANDMAX_H

#include <limits>

namespace mathem
{
	template <typename T>
	T min();

	template <typename T>
	T max();


	// IMPLEMENTATION BELOW //


	template <typename T>
	T min()
	{
		return std::numeric_limits<T>::min();
	}

	template <typename T>
	T max()
	{
		return std::numeric_limits<T>::max();
	}
}

#endif
