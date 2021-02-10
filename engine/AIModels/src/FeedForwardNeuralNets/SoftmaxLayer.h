
#ifndef SOFTMAXLAYER_H
#define SOFTMAXLAYER_H

namespace aimods
{
	template <typename T>
	class SoftmaxLayer
	{
	public:

		T* outputs = nullptr;
		
		SoftmaxLayer(size_t _size);
		~SoftmaxLayer();

		inline void resetOutputs();

		/// <summary>
		/// Calculates Softmax of inputs, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "size"</param>
		inline void propagate(T* inputs);

		// Getters //

		inline size_t getSize();

	private:

		size_t size = 0;
		size_t sizeInBytes = 0;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	SoftmaxLayer<T>::SoftmaxLayer(size_t _size)
	{
		size = _size;
		sizeInBytes = sizeof(T) * size;

		if (_size != 0)
		{
			outputs = (T*)realloc(outputs, sizeInBytes);
			resetOutputs();
		}
	}

	template <typename T>
	SoftmaxLayer<T>::~SoftmaxLayer()
	{
		free(outputs);
	}

	template <typename T>
	inline void SoftmaxLayer<T>::resetOutputs()
	{
		memset(outputs, 0, sizeInBytes);
	}

	template <typename T>
	inline void SoftmaxLayer<T>::propagate(T* inputs)
	{
		double sum = 0;
		for (size_t i = 0; i < size; i++)
		{
			sum += (double)inputs[i];
		}
		for (size_t i = 0; i < size; i++)
		{
			outputs[i] = (T)((double)inputs[i] / sum);
		}
	}

	template <typename T>
	inline size_t SoftmaxLayer<T>::getSize()
	{
		return size;
	}
}

#endif