
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
		T* thresholds = nullptr;
		T* weights = nullptr;

		activation function = activation::RELU;

		FullyConnectedLayer(activation _function, size_t _size, size_t _prevSize = 0);
		~FullyConnectedLayer();

		inline void setOutputs(T* values);
		inline void setWeights(T value = 0);

		/// <summary>
		/// Calculates weighted sum, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "prevSize"</param>
		inline void weighSum(T* inputs);

		/// <summary>
		/// Applies activation function to weighted sum stored in outputs (therefore
		/// weighSum(inputs) must be called first), then stores the results in outputs.
		/// </summary>
		inline void computeOutput();

		/// <summary>
		/// Propagates signals through the layer, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "prevSize"</param>
		inline void propagate(T* inputs);

		// Getters //

		inline size_t getSize();
		inline size_t getPrevSize();

	private:

		size_t size = 0;
		size_t prevSize = 0;
		size_t weightsSize = 0;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	FullyConnectedLayer<T>::FullyConnectedLayer(activation _function, size_t _size, size_t _prevSize)
	{
		size = _size;
		prevSize = _prevSize;
		weightsSize = size * prevSize;
		function = _function;

		if (_size != 0)
		{
			outputs = new T[size];
			thresholds = new T[size];

			size_t sizeInBytes = size * sizeof(T);
			memset(outputs, 0, sizeInBytes);
			memset(thresholds, 0, sizeInBytes);

			if (prevSize != 0)
			{
				weights = new T[weightsSize];
				memset(weights, 0, weightsSize * sizeof(T));
			}
			else
			{
				weights = nullptr;
			}
		}
	}

	template <typename T>
	FullyConnectedLayer<T>::~FullyConnectedLayer()
	{
		delete[] outputs;
		delete[] weights;
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::setOutputs(T* values)
	{
		for (size_t i = 0; i < size; i++)
		{
			outputs[i] = values[i];
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::setWeights(T value)
	{
		for (size_t i = 0; i < weightsSize; i++)
		{
			weights[i] = value;
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::weighSum(T* inputs)
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

	template <typename T>
	inline void FullyConnectedLayer<T>::computeOutput()
	{
		for (size_t i = 0; i < size; i++)
		{
			outputs[i] = (T)activationFunction((float)(outputs[i]), function);
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::propagate(T* inputs)
	{
		weighSum(inputs);
		computeOutput();
	}

	template <typename T>
	inline size_t FullyConnectedLayer<T>::getSize()
	{
		return size;
	}

	template <typename T>
	inline size_t FullyConnectedLayer<T>::getPrevSize()
	{
		return prevSize;
	}
}

#endif
