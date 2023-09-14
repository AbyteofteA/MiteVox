#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include "engine/CodeSafety/src/SafeArray.h"

#include <cstdint>
#include <utility>

namespace mathem
{
	template <typename T>
	inline void bubbleSort(safety::SafeArray<T>* arrayToSort, bool ascending = true);

	template <typename T>
	inline void bubbleSort(safety::SafeArray<T>* arrayToSort, safety::SafeArray<size_t>* resultIndeces, bool ascending = true);


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline void bubbleSort<T>(safety::SafeArray<T>* arrayToSort, bool ascending)
	{
		size_t arrayElementsCount = arrayToSort->getElementsCount() - 1;
		for (size_t repeat = 0; repeat < arrayElementsCount; ++repeat)
		{
			size_t lastElement = arrayElementsCount - repeat;
			for (size_t i = 0; i < lastElement; ++i)
			{
				T& left = (*arrayToSort)[i];
				T& right = (*arrayToSort)[i + 1];

				if (ascending)
				{
					if (left > right)
					{
						std::swap(left, right);
					}
				}
				else
				{
					if (left < right)
					{
						std::swap(left, right);
					}
				}
			}
		}
	}

	template <typename T>
	inline void bubbleSort<T>(safety::SafeArray<T>* arrayToSort, safety::SafeArray<size_t>* resultIndeces, bool ascending)
	{
		size_t arrayElementsCount = arrayToSort->getElementsCount();
		resultIndeces->resize(arrayElementsCount);
		for (size_t i = 0; i < arrayElementsCount; ++i)
		{
			resultIndeces->setElement(i, i);
		}

		arrayElementsCount -= 1;
		for (size_t repeat = 0; repeat < arrayElementsCount; ++repeat)
		{
			size_t lastElement = arrayElementsCount - repeat;
			for (size_t i = 0; i < lastElement; ++i)
			{
				T& left = (*arrayToSort)[i];
				T& right = (*arrayToSort)[i + 1];

				size_t& leftIndex = (*resultIndeces)[i];
				size_t& rightIndex = (*resultIndeces)[i + 1];

				if (ascending)
				{
					if (left > right)
					{
						std::swap(leftIndex, rightIndex);
					}
				}
				else
				{
					if (left < right)
					{
						std::swap(leftIndex, rightIndex);
					}
				}
			}
		}
	}
}

#endif