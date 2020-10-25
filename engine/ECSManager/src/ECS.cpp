
#ifndef ECS_CPP
#define ECS_CPP

#include <string>
#include <vector>
#include <queue>

/*****************************************************************************************
                  Implementation of Entity Component System pattern
                                    (single file)
*****************************************************************************************/

#define ENTITY_PACK 100000
// Restricts maximum amount of components.
#define COMPONENT_TYPE unsigned long
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
		COMPONENT_TYPE* IDtoComponent = NULL; // is not packed
		COMPONENT_TYPE* componentToID = NULL; // is packed

		void* data = NULL; // is packed
		COMPONENT_TYPE amountOfInstances = 0;

		ComponentManager() {}
		void init(std::string _name, COMPONENT_TYPE _ID, COMPONENT_SIZE_TYPE _sizeOfComponent)
		{
			name = _name;
			ID = _ID;

			sizeOfComponent = _sizeOfComponent;

			reserve(1, 1);
		}
		void wipe()
		{
			free(IDtoComponent);
			free(componentToID);
			free(data);
		}
		ComponentManager(std::string _name, COMPONENT_TYPE _ID, COMPONENT_SIZE_TYPE _sizeOfComponent)
		{
			init(_name, _ID,_sizeOfComponent);
		}
		~ComponentManager()
		{
			wipe();
		}

		// Utility methods //

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
		template <typename T>
		T* getComponentLocation(COMPONENT_TYPE ID)
		{
			if (exists(ID))
			{
				return (T*)data + IDtoComponent[ID];
			}
			return NULL;
		}
		void reserve(COMPONENT_TYPE _maxID, COMPONENT_TYPE _dataLength)
		{
			resizeIDtoComponent(_maxID);
			resizeData(_dataLength);
		}
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

			amountOfInstances++;

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
	ECS - the actual Entity Component System box.
	*****************************************************************************************/
	class ECS
	{

	public:

		ComponentManager componentManagers[MAX_COMPONENTS] = { ComponentManager() };
		MANAGER_INDEX_TYPE amountOfManagers = 0;

		void (*onCreate[MAX_COMPONENTS])(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index) = { NULL };
		void (*onUpdate[MAX_COMPONENTS])(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index) = { NULL };
		void (*onDestroy[MAX_COMPONENTS])(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index) = { NULL };

		Entity* entities = NULL;
		COMPONENT_TYPE amountOfEntities = 0;
		std::queue<COMPONENT_TYPE> availableIDs;

		COMPONENT_TYPE componentIDs = 0;

		void init()
		{
			for (COMPONENT_TYPE i = 0; i < ENTITY_PACK; i++)
			{
				availableIDs.push(i);
			}
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
		ECS()
		{
			init();
		}
		~ECS()
		{
			wipe();
		}

		// Component methods

		template <typename T>
		MANAGER_INDEX_TYPE registerComporent(std::string name,
			void (*_onCreate)(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index),
			void (*_onUpdate)(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index),
			void (*_onDestroy)(ECS* _ecs, MANAGER_INDEX_TYPE _managerIndex, COMPONENT_TYPE entityID, void* data, COMPONENT_TYPE index))
		{
			if (componentIDs == (~(COMPONENT_TYPE)0))
			{
				return -1;
			}

			COMPONENT_TYPE newID = 0;
			COMPONENT_TYPE availableIDs = ~componentIDs;
			MANAGER_INDEX_TYPE i = 0;
			for (; i < MAX_COMPONENTS; i++)
			{
				if (newID = availableIDs & (1 << i))
				{
					componentIDs |= newID;
					break;
				}
			}

			amountOfManagers++;
			componentManagers[i].init(name, newID, sizeof(T));
			onCreate[i] = _onCreate;
			onUpdate[i] = _onUpdate;
			onDestroy[i] = _onDestroy;

			return i;
		}
		bool unregisterComporent(MANAGER_INDEX_TYPE index)
		{
			if (1) // TOFIX: check if the component exists
			{
				componentIDs &= ~componentManagers[index].ID;
				
				for (COMPONENT_TYPE i = 0; i < amountOfEntities; i++)
				{
					detachComponent(i, index);
				}

				componentManagers[index].wipe();
				onCreate[index] = NULL;
				onUpdate[index] = NULL;
				onDestroy[index] = NULL;

				amountOfManagers--;
				
				return true;
			}
			
			return false;
		}
		
		// Entity methods

		void swapEntities(COMPONENT_TYPE index1, COMPONENT_TYPE index2)
		{
			Entity tmpEntity = {};
			tmpEntity = entities[index1];
			entities[index1] = entities[index2];
			entities[index2] = tmpEntity;
		}
		COMPONENT_TYPE addEntity()
		{
			COMPONENT_TYPE ID = availableIDs.front();
			availableIDs.pop();

			amountOfEntities++;
			entities = (Entity*)realloc(entities, sizeof(Entity) * amountOfEntities);
			entities[amountOfEntities - 1].ID = ID;
			entities[amountOfEntities - 1].components = 0;

			return amountOfEntities - 1;
		}
		bool deleteEntity(COMPONENT_TYPE index)
		{
			// Detach all entity's components.
			for (MANAGER_INDEX_TYPE i = 0; i < MAX_COMPONENTS; i++)
			{
				if (entities[index].components & (1 << i))
				{
					detachComponent(index, i);
				}
			}

			availableIDs.push(entities[index].ID);

			swapEntities(index, amountOfEntities - 1);

			amountOfEntities--;
			entities = (Entity*)realloc(entities, sizeof(Entity) * amountOfEntities);

			return true;
		}
		template <typename T>
		bool attachComponent(COMPONENT_TYPE entityIndex, MANAGER_INDEX_TYPE componentIndex, T* initializer = NULL)
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
				T* component = getComponent<T>(entityID, componentIndex);
				*component = *initializer;
			}
			COMPONENT_TYPE componentLocation = componentManagers[componentIndex].IDtoComponent[entityID];
			
			onCreate[componentIndex](this, componentIndex, entityID, initializer, componentLocation);

			return true;
		}
		void detachComponent(COMPONENT_TYPE entityIndex, MANAGER_INDEX_TYPE componentIndex)
		{
			COMPONENT_TYPE entityID = entities[entityIndex].ID;
			COMPONENT_TYPE componentLocation = componentManagers[componentIndex].IDtoComponent[entityID];

			onDestroy[componentIndex](this, componentIndex, entityID, NULL, componentLocation);
			
			entities[entityIndex].components &= ~componentManagers[componentIndex].ID;

			componentManagers[componentIndex].remove(entityID);
		}
		template <typename T>
		T* getComponent(COMPONENT_TYPE entityID, MANAGER_INDEX_TYPE componentIndex)
		{
			return componentManagers[componentIndex].getComponentLocation<T>(entityID);
		}

		// System methods

		void updateComponent(MANAGER_INDEX_TYPE componentIndex, void* data)
		{
			for (COMPONENT_TYPE c = 0; c < componentManagers[componentIndex].amountOfInstances; c++)
			{
				onUpdate[componentIndex](
						this, componentIndex, componentManagers[componentIndex].componentToID[c], data, c);
			}
		}
	};
}

#endif