
#ifndef SOFTMAXLAYER_H
#define SOFTMAXLAYER_H

namespace aimods
{
	template <typename T>
	class SoftmaxLayer
	{

	public:

		T* outputs = nullptr;
		size_t size = 0;
		size_t sizeInBytes = 0;

		SoftmaxLayer(size_t _size)
		{
			size = _size;
			sizeInBytes = sizeof(T) * size;

			if (_size != 0)
			{
				outputs = (T*)realloc(outputs, sizeInBytes);
				resetOutputs();
			}
		}
		~SoftmaxLayer()
		{
			free(outputs);
		}

		inline void resetOutputs()
		{
			memset(outputs, 0, sizeInBytes);
		}

		/// <summary>
		/// Calculates Softmax of inputs, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "size"</param>
		inline void propagate(T* inputs)
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
	};
}

#endif