#ifndef UNORDEREDSET_H
#define UNORDEREDSET_H

#include "engine/CodeSafety/src/SafeArray.h"

#include <cstdint>

namespace mathem
{
	template <class T>
	class UnorderedSet
	{
	public:

		inline void push(T element);
		inline void remove(T element);
		inline T popAny();
		inline void clear();
		inline size_t getElementsCount();
		inline T getElement(size_t index);
		inline bool contains(T element);
		inline void reserve(size_t elementsCount);
		inline void shrinkToFit();
		inline UnorderedSet<T>* copy();

	private:

		safety::SafeArray<T> elements;

		inline int64_t indexOf(T element);
	};


	// IMPLEMENTATION BELOW //


	template <class T>
	inline void UnorderedSet<T>::push(T element)
	{
		if (contains(element))
		{
			return;
		}
		elements.appendElement(element);
	}

	template <class T>
	inline void UnorderedSet<T>::remove(T element)
	{
		int64_t indexOfElement = indexOf(element);
		if (indexOfElement == -1)
		{
			return;
		}
		size_t indexOfLastElement = elements.getElementsCount() - 1;
		if (indexOfElement == indexOfLastElement)
		{
			elements.removeLastElement();
		}
		else
		{
			elements.removeElementAndSwapWithLast(indexOfElement);
		}
		return;
	}

	template <class T>
	inline T UnorderedSet<T>::popAny()
	{
		T element = elements.getLastElement();
		elements.removeLastElement();
		return element;
	}

	template <class T>
	inline void UnorderedSet<T>::clear()
	{
		elements.clear();
	}

	template <class T>
	inline size_t UnorderedSet<T>::getElementsCount()
	{
		return elements.getElementsCount();
	}

	template <class T>
	inline T UnorderedSet<T>::getElement(size_t index)
	{
		return elements.getElement(index);
	}

	template <class T>
	inline bool UnorderedSet<T>::contains(T element)
	{
		return indexOf(element) != -1;
	}

	template <class T>
	inline void UnorderedSet<T>::reserve(size_t elementsCount)
	{
		elements.reserve(elementsCount);
	}

	template <class T>
	inline void UnorderedSet<T>::shrinkToFit()
	{
		elements.shrinkToFit();
	}

	template <class T>
	inline UnorderedSet<T>* UnorderedSet<T>::copy()
	{
		UnorderedSet<T>* unorderedSetCopy = new UnorderedSet<T>();
		this->elements.copyTo(&unorderedSetCopy->elements);
		return unorderedSetCopy;
	}

	template <class T>
	inline int64_t UnorderedSet<T>::indexOf(T element)
	{
		size_t elementsCount = elements.getElementsCount();
		for (size_t i = 0; i < elementsCount; i++)
		{
			if (elements.getElement(i) == element)
			{
				return i;
			}
		}
		return -1;
	}
}
#endif

