
#ifndef UNIQUEIDGENERATOR_H
#define UNIQUEIDGENERATOR_H

#include <queue>

namespace mathem
{
	template <typename T>
	class UniqueIDGenerator
	{
		T maxID = 0;
		std::queue<T> returnedIDs;

	public:

		T getID()
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

		void returnID(T ID)
		{
			if (ID <= maxID)
			{
				returnedIDs.push(ID);
			}
		}

		void reset()
		{
			maxID = 0;
			while (!returnedIDs.empty())
			{
				returnedIDs.pop();
			}
		}
	};
}

#endif
