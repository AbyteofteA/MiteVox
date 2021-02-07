
#ifndef FULLYCONNECTEDLAYER_H
#define FULLYCONNECTEDLAYER_H

#include "AIModels/src/Structure/Functions.h"

namespace aimods
{
	template <typename T>
	class FullyConnectedLayer
	{

	public:

		T* outputs = nullptr;
		size_t size = 0;

		T* thresholds = nullptr;

		T* weights = nullptr;
		size_t prevSize = 0;
		size_t weightsSize = 0;

		activation function = 0;

		FullyConnectedLayer(activation _function, size_t _size, size_t _prevSize = 0)
		{
			size = _size;
			prevSize = _prevSize;
			weightsSize = size * prevSize;
			function = _function;

			if (_size != 0)
			{
				size_t sizeInBytes
					= sizeof(T) * size;

				outputs = (T*)realloc(outputs, sizeInBytes);
				thresholds = (T*)realloc(thresholds, sizeInBytes);

				memset(outputs, 0, sizeInBytes);
				memset(thresholds, 0, sizeInBytes);

				if (prevSize != 0)
				{
					size_t weightsSizeInBytes = sizeof(T) * weightsSize;
					weights = (T*)realloc(weights, weightsSizeInBytes);
					memset(weights, 0, weightsSizeInBytes);
				}
				else
				{
					free(weights);
					weights = nullptr;
				}
			}
		}
		~FullyConnectedLayer()
		{
			free(outputs);
			free(weights);
		}

		inline void setOutputs(T* values)
		{
			for (size_t i = 0; i < size; i++)
			{
				outputs[i] = values[i];
			}
		}
		inline void setWeights(T value = 0)
		{
			for (size_t i = 0; i < weightsSize; i++)
			{
				weights[i] = value;
			}
		}

		/// <summary>
		/// Calculates weighted sum, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "prevSize"</param>
		inline void weighSum(T* inputs)
		{
			for (size_t i = 0; i < size; i++)
			{
				T sum = 0;
				size_t weightsOffset = i * prevSize;

				for (size_t j = 0; j < prevSize; j++)
				{
					sum += inputs[j] * weights[j + weightsOffset];
				}
				sum -= thresholds[i];
				outputs[i] = sum;
			}
		}

		/// <summary>
		/// Applies activation function to weighted sum stored in outputs (therefore
		/// weighSum(inputs) must be called first), then stores the results in outputs.
		/// </summary>
		inline void computeOutput()
		{
			for (size_t i = 0; i < size; i++)
			{
				outputs[i] = (T)activationFunction((float)(outputs[i]), function);
			}
		}

		/// <summary>
		/// Propagates signals through the layer, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "prevSize"</param>
		inline void propagate(T* inputs)
		{
			weighSum(inputs);
			computeOutput();
		}
	};
}

#endif
