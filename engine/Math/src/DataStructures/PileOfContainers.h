#ifndef PILEOFCONTAINERS_H
#define PILEOFCONTAINERS_H

#include "engine/Math/src/DataStructures/UnorderedSet.h"
#include "engine/CodeSafety/src/SafeArray.h"

// Following defines work only this file (are undefined at the end).
#define PILEOFCONTAINERS_TEMPLATE template <class T, template<class Type> class ContainerType>
#define PILEOFCONTAINERS PileOfContainers<T, ContainerType>

namespace mathem
{
	PILEOFCONTAINERS_TEMPLATE
	class PileOfContainers
	{
	public:

		inline PileOfContainers(size_t initialPileSize, size_t initialContainerSize);
		inline void addContainer();
		inline void addUsedContainer(ContainerType<T>* usedContainer);

		inline ContainerType<T>* useContainer();
		inline void returnAllContainers();

		inline size_t getUsedContainersCount();
		inline ContainerType<T>* getUsedContainer(size_t index);

	private:

		size_t initialContainerSize;
		UnorderedSet<ContainerType<T>*> avalableContainers;
		UnorderedSet<ContainerType<T>*> usedContainers;
	};

	template <class T>
	using PileOfUnorderedSets = PileOfContainers<T, UnorderedSet>;

	template <class T>
	using PileOfSafeArrays = PileOfContainers<T, safety::SafeArray>;


	// IMPLEMENTATION BELOW //


	PILEOFCONTAINERS_TEMPLATE
	inline PILEOFCONTAINERS::PileOfContainers(size_t initialPileSize, size_t initialContainerSize)
	{
		this->initialContainerSize = initialContainerSize;
		for (size_t i = 0; i < initialPileSize; i++)
		{
			addContainer();
		}
		usedContainers.reserve(initialPileSize);
	}

	PILEOFCONTAINERS_TEMPLATE
	inline void PILEOFCONTAINERS::addContainer()
	{
		ContainerType<T>* newAvalableContainer = new ContainerType<T>();
		newAvalableContainer->reserve(this->initialContainerSize);
		avalableContainers.push(newAvalableContainer);
	}

	PILEOFCONTAINERS_TEMPLATE
	inline void PILEOFCONTAINERS::addUsedContainer(ContainerType<T>* usedContainer)
	{
		usedContainers.push(usedContainer);
	}

	PILEOFCONTAINERS_TEMPLATE
	inline ContainerType<T>* PILEOFCONTAINERS::useContainer()
	{
		if (avalableContainers.getElementsCount() == 0)
		{
			addContainer();
		}
		ContainerType<T>* container = avalableContainers.popAny();
		usedContainers.push(container);
		return container;
	}

	PILEOFCONTAINERS_TEMPLATE
	inline void PILEOFCONTAINERS::returnAllContainers()
	{
		while (usedContainers.getElementsCount() != 0)
		{
			ContainerType<T>* usedContainer = usedContainers.popAny();
			usedContainer->clear();
			avalableContainers.push(usedContainer);
		}
	}

	PILEOFCONTAINERS_TEMPLATE
	inline size_t PILEOFCONTAINERS::getUsedContainersCount()
	{
		return usedContainers.getElementsCount();
	}

	PILEOFCONTAINERS_TEMPLATE
	inline ContainerType<T>* PILEOFCONTAINERS::getUsedContainer(size_t index)
	{
		return usedContainers.getElement(index);
	}
}

#undef PILEOFCONTAINERS
#undef PILEOFCONTAINERS_TEMPLATE

#endif