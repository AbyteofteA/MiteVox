
#ifndef UNIQUEIDGENERATOR_H
#define UNIQUEIDGENERATOR_H

#include <queue>

namespace mathem
{
	template <typename T>
	class UniqueIDGenerator
	{
	public:

		UniqueIDGenerator();
		~UniqueIDGenerator();

		T getID();

		/// <summary>
		/// BUG: it accepts repetitions !
		/// </summary>
		void returnID(T ID);

		void reset();

	private:

		T maxID;
		std::queue<T> returnedIDs;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	UniqueIDGenerator<T>::UniqueIDGenerator()
	{
		maxID = 0;
	}

	template <typename T>
	UniqueIDGenerator<T>::~UniqueIDGenerator()
	{
		reset();
	}

	template <typename T>
	T UniqueIDGenerator<T>::getID()
	{
		if (returnedIDs.empty())
		{
			maxID += 1;
			return maxID;
		}
		else
		{
			T tmpID = returnedIDs.front();
			returnedIDs.pop();
			return tmpID;
		}
	}

	template <typename T>
	void UniqueIDGenerator<T>::returnID(T ID)
	{
		if (ID <= maxID)
		{
			returnedIDs.push(ID);
		}
	}

	template <typename T>
	void UniqueIDGenerator<T>::reset()
	{
		maxID = 0;
		while (!returnedIDs.empty())
		{
			returnedIDs.pop();
		}
	}
}

#endif
