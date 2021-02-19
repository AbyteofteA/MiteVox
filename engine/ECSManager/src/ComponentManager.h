
#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <string>

namespace ecs
{
	/// <summary>
	/// ComponentManager stores components well-packed.
	/// </summary>
	template <typename entityID>
	class ComponentManager
	{
	public:

		entityID ID = 0;
		std::string name;

		entityID maxID = 0; // actually stores max ID + 1
		entityID* IDtoComponent = nullptr; // is not packed
		entityID* componentToID = nullptr; // is packed

		void* data = nullptr; // is packed
		entityID amountOfInstances = 0;

		ComponentManager();
		ComponentManager(std::string _name, size_t _sizeOfComponent);
		~ComponentManager();

		// Utility methods //

		/// <summary>
		/// Checks if an entity with the ID exists.
		/// </summary>
		bool exists(entityID ID);

		void* getComponentLocation(entityID ID);

		/// <summary>
		/// Reserves memory to reduce allocations.
		/// </summary>
		void reserve(entityID _maxID, entityID _dataLength);

		/// <summary>
		/// Reallocates arrays to fit useful data.
		/// </summary>
		void fit();

		bool insert(entityID ID);
		bool remove(entityID ID);
		size_t getSizeOfComponent();

	private:

		size_t sizeOfComponent = 0;
		entityID dataLength = 0;

		entityID findMaxID();
		void swapComponents(entityID index1, entityID index2);

		// Resize methods //

		bool resizeIDtoComponent(entityID newMaxID);
		bool resizeData(entityID newDataLength);
	};


	// IMPLEMENTATION BELOW //


	template <typename entityID>
	ComponentManager<entityID>::ComponentManager() {}

	template <typename entityID>
	ComponentManager<entityID>::ComponentManager(std::string _name, size_t _sizeOfComponent)
	{
		name = _name;
		sizeOfComponent = _sizeOfComponent;
		reserve(1, 1);
	}

	template <typename entityID>
	ComponentManager<entityID>::~ComponentManager()
	{
		free(IDtoComponent);
		free(componentToID);
		free(data);
	}

	template <typename entityID>
	bool ComponentManager<entityID>::exists(entityID ID)
	{
		for (entityID i = 0; i < amountOfInstances; i++)
		{
			if (ID == componentToID[i])
			{
				return true;
			}
		}
		return false;
	}

	template <typename entityID>
	void* ComponentManager<entityID>::getComponentLocation(entityID ID)
	{
		if (exists(ID))
		{
			return (char*)data + sizeOfComponent * IDtoComponent[ID];
		}
		return nullptr;
	}

	template <typename entityID>
	void ComponentManager<entityID>::reserve(entityID _maxID, entityID _dataLength)
	{
		resizeIDtoComponent(_maxID);
		resizeData(_dataLength);
	}

	template <typename entityID>
	void ComponentManager<entityID>::fit()
	{
		resizeIDtoComponent(findMaxID());
		resizeData(amountOfInstances);
	}

	template <typename entityID>
	bool ComponentManager<entityID>::insert(entityID ID)
	{
		if (exists(ID))
		{
			return false;
		}

		entityID newLength = amountOfInstances + 1;
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

	template <typename entityID>
	bool ComponentManager<entityID>::remove(entityID ID)
	{
		if (!exists(ID))
		{
			return false;
		}

		entityID currentComponent = IDtoComponent[ID];
		entityID lastComponent = amountOfInstances - 1;
		entityID lastID = componentToID[lastComponent];

		// Swap current and the last component.
		swapComponents(currentComponent, lastComponent);

		// Update IDs.
		IDtoComponent[lastID] = currentComponent;
		componentToID[currentComponent] = lastID;

		amountOfInstances--;

		return true;
	}

	template <typename entityID>
	size_t ComponentManager<entityID>::getSizeOfComponent()
	{
		return sizeOfComponent;
	}

	template <typename entityID>
	entityID ComponentManager<entityID>::findMaxID()
	{
		entityID ID = 0;
		for (entityID i = 0; i < amountOfInstances; i++)
		{
			ID = (ID < componentToID[i]) ? componentToID[i] : ID;
		}
		return ID;
	}

	template <typename entityID>
	void ComponentManager<entityID>::swapComponents(entityID index1, entityID index2)
	{
		char tmp = 0;
		char* tmpObject1 = ((char*)data + index1 * sizeOfComponent);
		char* tmpObject2 = ((char*)data + index2 * sizeOfComponent);
		for (size_t i = 0; i < sizeOfComponent; i++)
		{
			tmp = tmpObject1[i];
			tmpObject1[i] = tmpObject2[i];
			tmpObject2[i] = tmp;
		}
	}

	template <typename entityID>
	bool ComponentManager<entityID>::resizeIDtoComponent(entityID newMaxID)
	{
		if (newMaxID <= findMaxID())
		{
			return false;
		}
		newMaxID++;
		IDtoComponent = (entityID*)realloc(IDtoComponent, sizeof(entityID) * newMaxID);
		maxID = newMaxID;

		return true;
	}

	template <typename entityID>
	bool ComponentManager<entityID>::resizeData(entityID newDataLength)
	{
		if ((dataLength == newDataLength) || (amountOfInstances > newDataLength))
		{
			return false;
		}
		componentToID = (entityID*)realloc(componentToID, sizeof(entityID) * newDataLength);
		data = realloc(data, sizeOfComponent * newDataLength);

		dataLength = newDataLength;

		return true;
	}
}

#endif