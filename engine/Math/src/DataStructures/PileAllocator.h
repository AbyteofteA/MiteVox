#ifndef PILEALLOCATOR_H
#define PILEALLOCATOR_H

#include "engine/CodeSafety/src/SafeArray.h"

namespace mathem
{
	template <class T>
	class PileAllocator
	{
	public:

		inline PileAllocator();
		inline void allocate(size_t elementsCount);
		inline void allocateNewElement();
		inline T* useElement();
		inline void returnUsedElement(T* usedElement);
		inline size_t getAvalableElementsCount();

	private:

		safety::SafeArray<T*> avalableElements;
	};


	// IMPLEMENTATION BELOW //


	template <class T>
	inline PileAllocator<T>::PileAllocator()
	{
		
	}

	template <class T>
	inline void PileAllocator<T>::allocate(size_t elementsCount)
	{
		avalableElements.reserve(elementsCount);
		for (size_t i = 0; i < elementsCount; i++)
		{
			allocateNewElement();
		}
	}

	template <class T>
	inline void PileAllocator<T>::allocateNewElement()
	{
		T* newAvalableElement = new T();
		avalableElements.appendElement(newAvalableElement);
	}

	template <class T>
	inline T* PileAllocator<T>::useElement()
	{
		if (avalableElements.getElementsCount() == 0)
		{
			allocateNewElement();
		}
		T* element = avalableElements.getLastElement();
		avalableElements.removeLastElement();
		return element;
	}

	template <class T>
	inline void PileAllocator<T>::returnUsedElement(T* usedElement)
	{
		avalableElements.appendElement(usedElement);
	}

	template <class T>
	inline size_t PileAllocator<T>::getAvalableElementsCount()
	{
		return avalableElements.getElementsCount();
	}
}

#endif
