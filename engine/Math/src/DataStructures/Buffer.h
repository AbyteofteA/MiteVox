
#ifndef BUFFER_H
#define BUFFER_H

#include <cstdlib>

namespace mathem
{
	template <typename T>
	class Buffer
	{

	private:

		unsigned int size = 0;
		unsigned int sizeData = 0;

	public:

		T* data = nullptr;

		Buffer()
		{
			size = 0;
			data = nullptr;
		}
		Buffer(unsigned int _size)
		{
			size = _size;
			data = (T*)malloc(sizeof(T) * size);
		}
		~Buffer()
		{
			wipe();
		}

		void resize(unsigned int _size)
		{
			if (size < _size)
			{
				size = _size;
				data = (T*)realloc(data, sizeof(T) * (size + 1));
			}
		}
		void fit()
		{
			resize(sizeData);
		}

		unsigned int getSizeData()
		{
			return sizeData;
		}
		void setSizeData(unsigned int _size)
		{
			if (size < _size)
			{
				resize(_size);
			}
			sizeData = _size;
		}
		void concat(Buffer* iBuffer)
		{
			unsigned int tmpIndex = sizeData;
			sizeData += iBuffer->sizeData;
			size = sizeData;
			data = (T*)realloc(data, sizeof(T) * size);
			for (int i = 0; i < iBuffer->sizeData; i++)
			{
				data[tmpIndex + i] = iBuffer->data[i];
			}
		}
		void wipe()
		{
			free(data);
			data = nullptr;
			size = 0;
			sizeData = 0;
		}
	};

	template<typename T>
	Buffer<T>** createManyBuffers(int n)
	{
		Buffer<T>** buffers = (Buffer<T>**)malloc(sizeof(Buffer<T>*) * n);
		for (int i = 0; i < n; i++)
		{
			buffers[i] = new Buffer<T>();
		}
		return buffers;
	}

}
#endif