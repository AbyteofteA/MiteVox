
#ifndef ENTITYCOMPONENTSYSTEM_H
#define ENTITYCOMPONENTSYSTEM_H

// Restricts maximum ComponentManager amount.
#define MANAGER_INDEX_TYPE unsigned char
#define MAX_COMPONENTS sizeof(entityID) * 8

#include "engine/ECSManager/src/Entity.h"
#include "engine/ECSManager/src/Prefab.h"
#include "engine/ECSManager/src/ComponentManager.h"

#include "engine/Math/src/Generators/UniqueIDGenerator.h"

#include <string>
#include <unordered_map>

namespace ecs
{
	template <typename entityID>
	class EntityComponentSystem
	{
	public:

		ComponentManager<entityID>* componentManagers[MAX_COMPONENTS] = { nullptr };

		EntityComponentSystem(entityID amount);
		~EntityComponentSystem();

		// Entity methods //

		/// <summary>
		/// Reserves memory for entities to reduce allocations.
		/// </summary>
		/// <param name="amount"></param>
		void reserveEntities(entityID amount);

		/// <summary>
		/// Reallocates arrays to fit useful entities.
		/// </summary>
		void fitEntities();
		void swapEntities(entityID index1, entityID index2);
		entityID getEntityIndex(entityID ID);
		entityID createEntity();

		/// <summary>
		/// Create Entity based on a prefab.
		/// </summary>
		entityID createEntity(entityID prefabID);
		void deleteEntity(entityID ID);
		void deleteEntityByIndex(entityID index);
		void deleteAllEntities();

		// Component methods //

		MANAGER_INDEX_TYPE registerComponent(std::string name, size_t componentSize,
			void (*_onCreate)(EntityComponentSystem* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index) = nullptr,
			void (*_onUpdateAll)(EntityComponentSystem* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data) = nullptr,
			void (*_onDestroy)(EntityComponentSystem* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index) = nullptr);
		bool unregisterComponent(MANAGER_INDEX_TYPE managerIndex);
		bool attachComponentByIndex(entityID entityIndex, MANAGER_INDEX_TYPE componentIndex, void* initializer = nullptr);
		bool attachComponent(entityID ID, MANAGER_INDEX_TYPE componentIndex, void* initializer = nullptr);
		void detachComponentByIndex(entityID entityIndex, MANAGER_INDEX_TYPE componentIndex);
		void detachComponent(entityID ID, MANAGER_INDEX_TYPE componentIndex);
		void* getComponent(entityID ID, MANAGER_INDEX_TYPE componentIndex);

		// Prefab methods //

		Prefab<entityID>* getPrefab(entityID ID);
		entityID createPrefab(std::string name);
		void deletePrefab(entityID ID);

		// System methods //

		void updateComponent(MANAGER_INDEX_TYPE componentIndex, void* data);

	private:

		entityID componentIDs = 0;

		void (*onCreate[MAX_COMPONENTS])(EntityComponentSystem* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index) = { nullptr };
		void (*onUpdateAll[MAX_COMPONENTS])(EntityComponentSystem* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data) = { nullptr };
		void (*onDestroy[MAX_COMPONENTS])(EntityComponentSystem* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index) = { nullptr };

		Entity<entityID>* entities = nullptr;
		entityID amountOfEntities = 0;
		entityID entitiesArraySize = 0;

		std::unordered_map<entityID, Prefab<entityID>*> prefabs;

		mathem::UniqueIDGenerator<entityID> IDGenerator;
	};


	// IMPLEMENTATION BELOW //


	template <typename entityID>
	EntityComponentSystem<entityID>::EntityComponentSystem(entityID amount)
	{
		for (MANAGER_INDEX_TYPE i = 0; i < MAX_COMPONENTS; i++)
		{
			componentManagers[i] = nullptr;

			onCreate[i] = nullptr;
			onUpdateAll[i] = nullptr;
			onDestroy[i] = nullptr;
		}
		reserveEntities(amount);
	}

	template <typename entityID>
	EntityComponentSystem<entityID>::~EntityComponentSystem()
	{
		free(entities);

		for (MANAGER_INDEX_TYPE i = 0; i < MAX_COMPONENTS; i++)
		{
			delete componentManagers[i];
		}
	}

	template <typename entityID>
	void EntityComponentSystem<entityID>::reserveEntities(entityID amount)
	{
		if (amount > amountOfEntities)
		{
			entitiesArraySize = amount;
			entities =
				(Entity<entityID>*)realloc(entities, sizeof(Entity<entityID>) * entitiesArraySize);
		}
	}

	template <typename entityID>
	void EntityComponentSystem<entityID>::fitEntities()
	{
		entitiesArraySize = amountOfEntities;
		entities =
			(Entity<entityID>*)realloc(entities, sizeof(Entity<entityID>) * entitiesArraySize);
	}

	template <typename entityID>
	void EntityComponentSystem<entityID>::swapEntities(entityID index1, entityID index2)
	{
		Entity<entityID> tmpEntity = {};
		tmpEntity = entities[index1];
		entities[index1] = entities[index2];
		entities[index2] = tmpEntity;
	}

	template <typename entityID>
	entityID EntityComponentSystem<entityID>::getEntityIndex(entityID ID)
	{
		for (entityID i = 0; i < amountOfEntities; i++)
		{
			if (entities[i].ID == ID)
			{
				return i;
			}
		}
		return 0;
	}

	template <typename entityID>
	entityID EntityComponentSystem<entityID>::createEntity()
	{
		entityID ID = IDGenerator.getID();

		entityID entityIndex = amountOfEntities;
		amountOfEntities++;

		// Realloc entities if the buffer is not big enough.
		if (amountOfEntities > entitiesArraySize)
		{
			entitiesArraySize = amountOfEntities;
			entities =
				(Entity<entityID>*)realloc(entities, sizeof(Entity<entityID>) * entitiesArraySize);
		}

		entities[entityIndex].ID = ID;
		entities[entityIndex].components = 0;

		return ID;
	}

	template <typename entityID>
	entityID EntityComponentSystem<entityID>::createEntity(entityID prefabID)
	{
		entityID ID = IDGenerator.getID();

		entityID entityIndex = amountOfEntities;
		amountOfEntities++;

		// Realloc entities if the buffer is not big enough.
		if (amountOfEntities > entitiesArraySize)
		{
			entitiesArraySize = amountOfEntities;
			entities =
				(Entity<entityID>*)realloc(entities, sizeof(Entity<entityID>) * entitiesArraySize);
		}

		entities[entityIndex].ID = ID;
		entities[entityIndex].components = 0;

		Prefab<entityID>* prefab = getPrefab(prefabID);
		for (MANAGER_INDEX_TYPE i = 0; i < MAX_COMPONENTS; i++)
		{
			if (prefab->entity.components & (1 << i))
			{
				attachComponentByIndex(entityIndex, i, prefab->components[i]);
			}
		}

		return ID;
	}

	template <typename entityID>
	void EntityComponentSystem<entityID>::deleteEntity(entityID ID)
	{
		entityID entityIndex = getEntityIndex(ID);
		deleteEntityByIndex(entityIndex);
	}

	template <typename entityID>
	void EntityComponentSystem<entityID>::deleteEntityByIndex(entityID index)
	{
		// Detach all entity's components.
		for (MANAGER_INDEX_TYPE i = 0; i < MAX_COMPONENTS; i++)
		{
			if (entities[index].components & (1 << i))
			{
				detachComponentByIndex(index, i);
			}
		}

		IDGenerator.returnID(entities[index].ID);

		entityID maxIndex = amountOfEntities - 1;
		swapEntities(index, maxIndex);

		amountOfEntities--;
	}

	template <typename entityID>
	void EntityComponentSystem<entityID>::deleteAllEntities()
	{
		for (entityID i = 0; i < amountOfEntities; i++)
		{
			deleteEntityByIndex(i);
		}
	}

	template <typename entityID>
	MANAGER_INDEX_TYPE EntityComponentSystem<entityID>::registerComponent(std::string name, size_t componentSize,
		void (*_onCreate)(EntityComponentSystem* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index),
		void (*_onUpdateAll)(EntityComponentSystem* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data),
		void (*_onDestroy)(EntityComponentSystem* _ecs, MANAGER_INDEX_TYPE _managerIndex, entityID ID, void* data, entityID index))
	{
		if (componentIDs == (~(entityID)0))
		{
			return 0;
		}

		entityID newID = 0;
		entityID availableIDs = ~componentIDs;
		MANAGER_INDEX_TYPE managerIndex = 0;
		for (; managerIndex < MAX_COMPONENTS; managerIndex++)
		{
			if (newID = availableIDs & (1 << managerIndex))
			{
				break;
			}
		}

		componentManagers[managerIndex] = new ComponentManager<entityID>(name, componentSize);

		componentManagers[managerIndex]->ID = newID;
		componentIDs |= newID;

		onCreate[managerIndex] = _onCreate;
		onUpdateAll[managerIndex] = _onUpdateAll;
		onDestroy[managerIndex] = _onDestroy;

		return managerIndex;
	}

	template <typename entityID>
	bool EntityComponentSystem<entityID>::unregisterComponent(MANAGER_INDEX_TYPE managerIndex)
	{
		if (componentIDs & (1 << managerIndex))
		{
			componentIDs &= ~componentManagers[managerIndex]->ID;

			for (entityID i = 0; i < amountOfEntities; i++)
			{
				detachComponent(i, managerIndex);
			}

			delete componentManagers[managerIndex];
			componentManagers[managerIndex] = nullptr;

			onCreate[managerIndex] = nullptr;
			onUpdateAll[managerIndex] = nullptr;
			onDestroy[managerIndex] = nullptr;

			return true;
		}

		return false;
	}

	template <typename entityID>
	bool EntityComponentSystem<entityID>::attachComponentByIndex(entityID entityIndex, MANAGER_INDEX_TYPE componentIndex, void* initializer)
	{
		entityID ID = entities[entityIndex].ID;
		if (entities[entityIndex].components & componentManagers[componentIndex]->ID)
		{
			return false;
		}
		entities[entityIndex].components |= componentManagers[componentIndex]->ID;
		componentManagers[componentIndex]->insert(ID);

		if (initializer)
		{
			void* component = getComponent(ID, componentIndex);
			size_t size = componentManagers[componentIndex]->getSizeOfComponent();
			int res = memcmp(component, initializer, size);
			memcpy(component, initializer, size);
			res = memcmp(component, initializer, size);
			res = res;
		}
		entityID componentLocation = componentManagers[componentIndex]->IDtoComponent[ID];

		if (onCreate[componentIndex])
		{
			onCreate[componentIndex](this, componentIndex, ID, initializer, componentLocation);
		}

		return true;
	}

	template <typename entityID>
	bool EntityComponentSystem<entityID>::attachComponent(entityID ID, MANAGER_INDEX_TYPE componentIndex, void* initializer)
	{
		entityID entityIndex = getEntityIndex(ID);
		return attachComponentByIndex(entityIndex, componentIndex, initializer);
	}

	template <typename entityID>
	void EntityComponentSystem<entityID>::detachComponentByIndex(entityID entityIndex, MANAGER_INDEX_TYPE componentIndex)
	{
		entityID ID = entities[entityIndex].ID;
		entityID componentLocation = componentManagers[componentIndex]->IDtoComponent[ID];

		if (onDestroy[componentIndex])
		{
			onDestroy[componentIndex](this, componentIndex, ID, nullptr, componentLocation);
		}

		entities[entityIndex].components &= ~componentManagers[componentIndex]->ID;

		componentManagers[componentIndex]->remove(ID);
	}

	template <typename entityID>
	void EntityComponentSystem<entityID>::detachComponent(entityID ID, MANAGER_INDEX_TYPE componentIndex)
	{
		entityID entityIndex = getEntityIndex(ID);
		detachComponentByIndex(entityIndex, componentIndex);
	}

	template <typename entityID>
	void* EntityComponentSystem<entityID>::getComponent(entityID ID, MANAGER_INDEX_TYPE componentIndex)
	{
		return componentManagers[componentIndex]->getComponentLocation(ID);
	}

	template <typename entityID>
	Prefab<entityID>* EntityComponentSystem<entityID>::getPrefab(entityID ID)
	{
		return prefabs[ID];
	}

	template <typename entityID>
	entityID EntityComponentSystem<entityID>::createPrefab(std::string name)
	{
		entityID ID = IDGenerator.getID();
		prefabs.insert( { ID, new ecs::Prefab<entityID>(name, ID) } );

		return ID;
	}

	template <typename entityID>
	void EntityComponentSystem<entityID>::deletePrefab(entityID ID)
	{
		delete prefabs[ID];
		IDGenerator.returnID(ID);
		prefabs.erase(ID);
	}

	template <typename entityID>
	void EntityComponentSystem<entityID>::updateComponent(MANAGER_INDEX_TYPE componentIndex, void* data)
	{
		onUpdateAll[componentIndex](this, componentIndex, data);
	}
}

#endif
