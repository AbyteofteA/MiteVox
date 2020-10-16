
#ifndef HALFTABLE_H
#define HALFTABLE_H


namespace mathem
{
	template <typename T>
	class HalfTable
	{
		unsigned int size;
		unsigned long sizeData;
		T* data = nullptr;

		HalfTable(unsigned int _size)
		{
			resize(_size);
		}
		~HalfTable()
		{
			free(data);
		}

		void resize(unsigned int _size)
		{

			size = _size;

			sizeData = (unsigned long)getArithmProgressionSum(1, 1, size);
			free(data);
			data = (T*)calloc(data, sizeof(T) * sizeData);
		}

		T get(unsigned int x, unsigned int y)
		{
			unsigned long XPlusY = x + y;
			unsigned long index = x * size + y;
			unsigned long sizeMinus1 = size - 1;
			if (index > ((unsigned long)size * 2 - 2))
			{
				return { 0 };
			}
			if (XPlusY > sizeMinus1)
			{
				x = sizeMinus1 - x;
				y = sizeMinus1 - y;
			}

			return data[index];
		}
		void set(unsigned int x, unsigned int y, T value)
		{
			unsigned long XPlusY = x + y;
			unsigned long index = x * size + y;
			unsigned long sizeMinus1 = size - 1;
			if (index > ((unsigned long)size * 2 - 2))
			{
				return { 0 };
			}
			if (XPlusY > sizeMinus1)
			{
				x = sizeMinus1 - x;
				y = sizeMinus1 - y;
			}

			data[index] = value;
		}
	};
}


#endif