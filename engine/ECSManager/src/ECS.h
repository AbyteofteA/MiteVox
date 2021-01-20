
#ifndef ECS_H
#define ECS_H

#include <string>
#include <vector>
#include <queue>

/*****************************************************************************************
                  Implementation of Entity-Component-System pattern
                                    (single file)
*****************************************************************************************/

#define ENTITY_PACK 100000
// Restricts maximum amount of components.
#define COMPONENT_TYPE unsigned long
#define ENTITY_ID_TYPE COMPONENT_TYPE
// Restricts maximum component size.
#define COMPONENT_SIZE_TYPE unsigned long
// Restricts maximum ComponentManager amount.
#define MANAGER_INDEX_TYPE unsigned char
#define MAX_COMPONENTS sizeof(COMPONENT_TYPE) * 8

namespace ecs
{
	/*****************************************************************************************
	Entity is just an unique ID + component flag.
	*****************************************************************************************/
	struct Entity
	{
		COMPONENT_TYPE ID = 0;
		COMPONENT_TYPE components = 0;
	};
	

	/*****************************************************************************************
	ComponentManager stores components well-packed.
	*****************************************************************************************/
	class ComponentManager
	{

	public:

		COMPONENT_TYPE ID = 0;
		std::string name;

		COMPONENT_TYPE maxID = 0; // actually stores max ID + 1
		COMPONENT_TYPE* IDtoComponent = nullptr; // is not packed
		COMPONENT_TYPE* componentToID = nullptr; // is packed

		void* data = nullptr; // is packed
		COMPONENT_TYPE amountOfInstances = 0;

		ComponentManager() {}
		ComponentManager(std::string _name, COMPONENT_SIZE_TYPE _sizeOfComponent)
		{
			init(_name, _sizeOfComponent);
		}
		~ComponentManager()
		{
			wipe();
		}

		void init(std::string _name, COMPONENT_SIZE_TYPE _sizeOfComponent)
		{
			name = _name;

			sizeOfComponent = _sizeOfComponent;

			reserve(1, 1);
		}
		void wipe()
		{
			free(IDtoComponent);
			free(componentToID);
			free(data);
		}

		// Utility methods //

		/*****************************************************************************************
		Checks if an entity with the ID exists.
		*****************************************************************************************/
		bool exists(COMPONENT_TYPE ID)
		{
			for (COMPONENT_TYPE i = 0; i < amountOfInstances; i++)
			{
				if (ID == componentToID[i])
				{
					return true;
				}
			}
			return false;
		}

		void* getComponentLocation(COMPONENT_TYPE ID)
		{
			if (exists(ID))
			{
				return (char*)data + sizeOfComponent * IDtoComponent[ID];
			}
			return nullptr;
		}
		
		/*****************************************************************************************
		Reserves memory to reduce allocations.
		*****************************************************************************************/
		void reserve(COMPONENT_TYPE _maxID, COMPONENT_TYPE _dataLength)
		{
			resizeIDtoComponent(_maxID);
			resizeData(_dataLength);
		}
		
		/*****************************************************************************************
		Reallocates arrays to fit useful data.
		*****************************************************************************************/
		void fit()
		{
			resizeIDtoComponent(findMaxID());
			resizeData(amountOfInstances);
		}

		bool insert(COMPONENT_TYPE ID)
		{
			if (exists(ID))
			{
				return false;
			}

			COMPONENT_TYPE newLength = amountOfInstances + 1;
			if (dataLength < newLength)
			{
				resizeData(newLength);
			}
			resizeIDtoComponent(ID);

			IDtoComponent[ID] = amountOfInstances;
			componentToID[amountOfInstances] = ID;

			amountOfInstances = newLength;

			return true;
		}

		bool remove(COMPONENT_TYPE ID)
		{
			if (!exists(ID))
			{
				return false;
			}

			COMPONENT_TYPE currentComponent = IDtoComponent[ID];
			COMPONENT_TYPE lastComponent = amountOfInstances - 1;
			COMPONENT_TYPE lastID = componentToID[lastComponent];

			// Swap current and the last component.
			swapComponents(currentComponent, lastComponent);

			// Update IDs.
			IDtoComponent[lastID] = currentComponent;
			componentToID[currentComponent] = lastID;

			amountOfInstances--;

			return true;
		}

		COMPONENT_SIZE_TYPE getSizeOfComponent()
		{
			return sizeOfComponent;
		}

	private:

		COMPONENT_SIZE_TYPE sizeOfComponent = 0;

		COMPONENT_TYPE dataLength = 0;

		COMPONENT_TYPE findMaxID()
		{
			COMPONENT_TYPE ID = 0;
			for (COMPONENT_TYPE i = 0; i < amountOfInstances; i++)
			{
				ID = (ID < componentToID[i]) ? componentToID[i] : ID;
			}
			return ID;
		}

		void swapComponents(COMPONENT_TYPE index1, COMPONENT_TYPE index2)
		{
			char tmp = 0;
			char* tmpObject1 = ((char*)data + index1 * sizeOfComponent);
			char* tmpObject2 = ((char*)data + index2 * sizeOfComponent);
			for (COMPONENT_SIZE_TYPE i = 0; i < sizeOfComponent; i++)
			{
				tmp = tmpObject1[i];
				tmpObject1[i] = tmpObject2[i];
				tmpObject2[i] = tmp;
			}
		}

		// Resize methods //

		bool resizeIDtoComponent(COMPONENT_TYPE newMaxID)
		{
			if (newMaxID <= findMaxID())
			{
				return false;
			}
			newMaxID++;
			IDtoComponent = (COMPONENT_TYPE*)realloc(IDtoComponent, sizeof(COMPONENT_TYPE) * newMaxID);
			maxID = newMaxID;

			return true;
		}

		bool resizeData(COMPONENT_TYPE newDataLength)
		{
			if ((dataLength == newDataLength) || (amountOfInstances > newDataLength))
			{
				return false;
			}
			componentToID = (COMPONENT_TYPE*)realloc(componentToID, sizeof(COMPONENT_TYPE) * newDataLength);
			data = realloc(data, sizeOfComponent * newDataLength);

			dataLength = newDataLength;

			return true;
		}
	};


	/*****************************************************************************************
	Prefab implements prefabrecated entities. 
	*****************************************************************************************/
	class Prefab
	{

	public:

		std::string name;
		Entity entity;
		void* components[MAX_COMPONENTS];

		Prefab(std::string _name, COMPONENT_TYPE ID)
		{
			name = _name;
			init(ID);
		}
		~Prefab()
		{
			wipe();
		}

		void init(COMPONENT_TYPE ID)
		{
			entity.ID = ID;
			for (int i = 0; i < MAX_COMPONENTS; i++)
			{
				components[i] = nullptr;
			}
			entity.components = 0;
		}
		void wipe()
		{
			for (int i = 0; i < MAX_COMPONENTS; i++)
			{
				if (entity.components & (1 << i))
				{
					free(components[i]);
				}
				components[i] = nullptr;
			}
			entity.components = 0;
		}

		int attachComponent(ComponentManager* manager, void* initializer = nullptr)
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
				COMPONENT_SIZE_TYPE size = manager->getSizeOfComponent();
				entity.components |= manager->ID;
				components[index] = nullptr;
				if (initializer)
				{
					components[index] = malloc(size);
					memcpy(components[index], initializer, size);
				}
				return 1;
			}
			else
			{
				return 0;
			}
		}

		int detachComponent(ComponentManager* manager)
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
				free(components[index]);
				components[index] = nullptr;
				entity.components &= ~manager->ID;
				return 1;
			}
			else
			{
				return 0;
			}
		}
	};


	/*****************************************************************************************
	ECS - Entity-Component-System implementation.
	*****************************************************************************************/
	class ECS
	{

	public:

		ComponentManager componentManagers[MAX_COMPONENTS] = { ComponentManager() };
		COMPONENT_TYPE componentIDs = 0;

		void (*onCreate[MAX_COMPONENTS])(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index) = { nullptr };
		void (*onUpdateAll[MAX_COMPONENTS])(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data) = { nullptr };
		void (*onDestroy[MAX_COMPONENTS])(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index) = { nullptr };

		Entity* entities = nullptr;
		COMPONENT_TYPE amountOfEntities = 0;
		COMPONENT_TYPE entitiesArraySize = 0;
		std::queue<COMPONENT_TYPE> availableIDs;

		std::vector<Prefab*> prefabs;

		ECS(COMPONENT_TYPE amount)
		{
			init(amount);
		}
		~ECS()
		{
			wipe();
		}

		void init(COMPONENT_TYPE amount)
		{
			for (COMPONENT_TYPE i = 0; i < ENTITY_PACK; i++)
			{
				availableIDs.push(i);
			}
			for (MANAGER_INDEX_TYPE i = 0; i < MAX_COMPONENTS; i++)
			{
				onCreate[i] = nullptr;
				onUpdateAll[i] = nullptr;
				onDestroy[i] = nullptr;
			}
			reserveEntities(amount);
		}
		void wipe()
		{
			free(entities);

			for (MANAGER_INDEX_TYPE i = 0; i < MAX_COMPONENTS; i++)
			{
				componentManagers[i].wipe();
			}

			while (!availableIDs.empty())
			{
				availableIDs.pop();
			}
		}

		// Entity methods //

		/*****************************************************************************************
		Reserves memory for entities to reduce allocations.
		*****************************************************************************************/
		void reserveEntities(COMPONENT_TYPE amount)
		{
			if (amount > amountOfEntities)
			{
				entitiesArraySize = amount;
				entities = (Entity*)realloc(entities, sizeof(Entity) * entitiesArraySize);
			}
		}

		/*****************************************************************************************
		Reallocates arrays to fit useful entities.
		*****************************************************************************************/
		void fitEntities()
		{
			entitiesArraySize = amountOfEntities;
			entities = (Entity*)realloc(entities, sizeof(Entity) * entitiesArraySize);
		}

		void swapEntities(COMPONENT_TYPE index1, COMPONENT_TYPE index2)
		{
			Entity tmpEntity = {};
			tmpEntity = entities[index1];
			entities[index1] = entities[index2];
			entities[index2] = tmpEntity;
		}

		COMPONENT_TYPE getEntityIndex(COMPONENT_TYPE ID)
		{
			for (COMPONENT_TYPE i = 0; i < amountOfEntities; i++)
			{
				if (entities[i].ID == ID)
				{
					return i;
				}
			}
			return 0;
		}

		COMPONENT_TYPE createEntity()
		{
			COMPONENT_TYPE ID = availableIDs.front();
			availableIDs.pop();

			COMPONENT_TYPE entityIndex = amountOfEntities;
			amountOfEntities++;

			// Realloc entities if the buffer is not big enough.
			if (amountOfEntities > entitiesArraySize)
			{
				entitiesArraySize = amountOfEntities;
				entities = (Entity*)realloc(entities, sizeof(Entity) * entitiesArraySize);
			}

			entities[entityIndex].ID = ID;
			entities[entityIndex].components = 0;

			return ID;
		}

		/*****************************************************************************************
		Create Entity based on a prefab.
		*****************************************************************************************/
		COMPONENT_TYPE createEntity(COMPONENT_TYPE prefabID)
		{
			COMPONENT_TYPE ID = availableIDs.front();
			availableIDs.pop();

			COMPONENT_TYPE entityIndex = amountOfEntities;
			amountOfEntities++;
			
			// Realloc entities if the buffer is not big enough.
			if (amountOfEntities > entitiesArraySize)
			{
				entitiesArraySize = amountOfEntities;
				entities = (Entity*)realloc(entities, sizeof(Entity) * entitiesArraySize);
			}

			entities[entityIndex].ID = ID;
			entities[entityIndex].components = 0;

			Prefab* prefab = getPrefab(prefabID);
			for (MANAGER_INDEX_TYPE i = 0; i < MAX_COMPONENTS; i++)
			{
				if (prefab->entity.components & (1 << i))
				{
					attachComponentByIndex(entityIndex, i, prefab->components[i]);
				}
			}

			return ID;
		}

		void deleteEntity(COMPONENT_TYPE entityID)
		{
			COMPONENT_TYPE entityIndex = getEntityIndex(entityID);
			deleteEntityByIndex(entityIndex);
		}

		void deleteEntityByIndex(COMPONENT_TYPE entityIndex)
		{
			// Detach all entity's components.
			for (MANAGER_INDEX_TYPE i = 0; i < MAX_COMPONENTS; i++)
			{
				if (entities[entityIndex].components & (1 << i))
				{
					detachComponentByIndex(entityIndex, i);
				}
			}

			availableIDs.push(entities[entityIndex].ID);

			COMPONENT_TYPE maxIndex = amountOfEntities - 1;
			swapEntities(entityIndex, maxIndex);

			amountOfEntities--;
		}

		void deleteAllEntities()
		{
			for (COMPONENT_TYPE i = 0; i < amountOfEntities; i++)
			{
				deleteEntityByIndex(i);
			}
		}

		// Component methods //

		template <typename T>
		MANAGER_INDEX_TYPE registerComponent(std::string name,
			void (*_onCreate)(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index) = nullptr,
			void (*_onUpdateAll)(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, void* data) = nullptr,
			void (*_onDestroy)(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index) = nullptr)
		{
			if (componentIDs == (~(COMPONENT_TYPE)0))
			{
				return 0;
			}

			COMPONENT_TYPE newID = 0;
			COMPONENT_TYPE availableIDs = ~componentIDs;
			MANAGER_INDEX_TYPE managerIndex = 0;
			for (; managerIndex < MAX_COMPONENTS; managerIndex++)
			{
				if (newID = availableIDs & (1 << managerIndex))
				{
					break;
				}
			}
			componentManagers[managerIndex].ID = newID;
			componentIDs |= newID;

			componentManagers[managerIndex].init(name, sizeof(T));
			onCreate[managerIndex] = _onCreate;
			onUpdateAll[managerIndex] = _onUpdateAll;
			onDestroy[managerIndex] = _onDestroy;

			return managerIndex;
		}

		bool unregisterComponent(MANAGER_INDEX_TYPE managerIndex)
		{
			if (componentIDs & (1 << managerIndex))
			{
				componentIDs &= ~componentManagers[managerIndex].ID;

				for (COMPONENT_TYPE i = 0; i < amountOfEntities; i++)
				{
					detachComponent(i, managerIndex);
				}

				componentManagers[managerIndex].wipe();
				onCreate[managerIndex] = nullptr;
				onUpdateAll[managerIndex] = nullptr;
				onDestroy[managerIndex] = nullptr;

				return true;
			}

			return false;
		}

		bool attachComponentByIndex(COMPONENT_TYPE entityIndex, MANAGER_INDEX_TYPE componentIndex, void* initializer = nullptr)
		{
			COMPONENT_TYPE entityID = entities[entityIndex].ID;
			if (entities[entityIndex].components & componentManagers[componentIndex].ID)
			{
				return false;
			}
			entities[entityIndex].components |= componentManagers[componentIndex].ID;
			componentManagers[componentIndex].insert(entityID);

			if (initializer)
			{
				void* component = getComponent(entityID, componentIndex);
				COMPONENT_SIZE_TYPE size = componentManagers[componentIndex].getSizeOfComponent();
				int res = memcmp(component, initializer, size);
				memcpy(component, initializer, size);
				res = memcmp(component, initializer, size);
				res = res;
			}
			COMPONENT_TYPE componentLocation = componentManagers[componentIndex].IDtoComponent[entityID];

			if (onCreate[componentIndex])
			{
				onCreate[componentIndex](this, componentIndex, entityID, initializer, componentLocation);
			}

			return true;
		}
		bool attachComponent(COMPONENT_TYPE entityID, MANAGER_INDEX_TYPE componentIndex, void* initializer = nullptr)
		{
			COMPONENT_TYPE entityIndex = getEntityIndex(entityID);
			return attachComponentByIndex(entityIndex, componentIndex, initializer);
		}

		void detachComponentByIndex(COMPONENT_TYPE entityIndex, MANAGER_INDEX_TYPE componentIndex)
		{
			COMPONENT_TYPE entityID = entities[entityIndex].ID;
			COMPONENT_TYPE componentLocation = componentManagers[componentIndex].IDtoComponent[entityID];

			if (onDestroy[componentIndex])
			{
				onDestroy[componentIndex](this, componentIndex, entityID, nullptr, componentLocation);
			}

			entities[entityIndex].components &= ~componentManagers[componentIndex].ID;

			componentManagers[componentIndex].remove(entityID);
		}
		void detachComponent(COMPONENT_TYPE entityID, MANAGER_INDEX_TYPE componentIndex)
		{
			COMPONENT_TYPE entityIndex = getEntityIndex(entityID);
			detachComponentByIndex(entityIndex, componentIndex);
		}

		void* getComponent(COMPONENT_TYPE entityID, MANAGER_INDEX_TYPE componentIndex)
		{
			return componentManagers[componentIndex].getComponentLocation(entityID);
		}

		// Prefab methods //

		Prefab* getPrefab(COMPONENT_TYPE ID)
		{
			for (auto prefab : prefabs)
			{
				if (prefab->entity.ID == ID)
				{
					return prefab;
				}
			}
			return nullptr;
		}

		COMPONENT_TYPE getPrefabIndex(COMPONENT_TYPE ID)
		{
			for (COMPONENT_TYPE i = 0; i < prefabs.size(); i++)
			{
				if (prefabs[i]->entity.ID == ID)
				{
					return i;
				}
			}
			return 0;
		}

		COMPONENT_TYPE createPrefab(std::string name)
		{
			COMPONENT_TYPE ID = availableIDs.front();
			availableIDs.pop();
			prefabs.push_back(new ecs::Prefab(name, ID));

			return ID;
		}

		void deletePrefab(COMPONENT_TYPE ID)
		{
			Prefab* prefab = getPrefab(ID);
			delete prefab;
			availableIDs.push(prefab->entity.ID);
			prefabs.erase(prefabs.begin() + getPrefabIndex(ID));
		}

		// System methods //

		void updateComponent(MANAGER_INDEX_TYPE componentIndex, void* data)
		{
			onUpdateAll[componentIndex](this, componentIndex, data);
		}
	};
}

#endif
