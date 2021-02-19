
#ifndef PREFAB_H
#define PREFAB_H

#include "ComponentManager.h"

#include <string>

namespace ecs
{
	/// <summary>
	/// Prefab implements prefabrecated entities.
	/// </summary>
	template <typename entityID>
	class Prefab
	{
	public:

		std::string name;
		Entity<entityID> entity;
		void* components[MAX_COMPONENTS];

		Prefab(std::string _name, entityID ID);
		~Prefab();

		int attachComponent(ComponentManager<entityID>* manager, void* initializer = nullptr);
		int detachComponent(ComponentManager<entityID>* manager);
	};


	// IMPLEMENTATION BELOW //


	template <typename entityID>
	Prefab<entityID>::Prefab(std::string _name, entityID ID)
	{
		name = _name;
		entity.ID = ID;
		for (int i = 0; i < MAX_COMPONENTS; i++)
		{
			components[i] = nullptr;
		}
		entity.components = 0;
	}

	template <typename entityID>
	Prefab<entityID>::~Prefab()
	{
		for (int i = 0; i < MAX_COMPONENTS; i++)
		{
			if (entity.components & (1 << i))
			{
				delete components[i];
			}
			components[i] = nullptr;
		}
		entity.components = 0;
	}

	template <typename entityID>
	int Prefab<entityID>::attachComponent(ComponentManager<entityID>* manager, void* initializer)
	{
		int index = -1;
		for (int i = 0; i < MAX_COMPONENTS; i++)
		{
			if (manager->ID == (1 << i))
			{
				index = i;
				break;
			}
		}

		if (index >= 0)
		{
			size_t size = manager->getSizeOfComponent();
			entity.components |= manager->ID;
			components[index] = nullptr;
			if (initializer)
			{
				components[index] = (void*)new char[size];
				memcpy(components[index], initializer, size);
			}
			return 1;
		}
		else
		{
			return 0;
		}
	}

	template <typename entityID>
	int Prefab<entityID>::detachComponent(ComponentManager<entityID>* manager)
	{
		int index = -1;
		for (int i = 0; i < MAX_COMPONENTS; i++)
		{
			if (manager->ID == (1 << i))
			{
				index = i;
				break;
			}
		}

		if (index >= 0)
		{
			delete components[index];
			components[index] = nullptr;
			entity.components &= ~manager->ID;
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

#endif