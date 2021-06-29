
#ifndef SOFTMAXLAYER_H
#define SOFTMAXLAYER_H

#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"

namespace aimods
{
	template <typename T>
	class SoftmaxLayer : public NeuralNetworkLayerBase<T>
	{
	public:

		SoftmaxLayer(size_t outputsCount);
		~SoftmaxLayer();

		inline void setWeightsRandom() {};
		inline void setWeights(T value) {};

		/// <summary>
		/// Calculates Softmax of inputs, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "size"</param>
		inline void propagate(T* inputs);
		inline void propagateSavingWeightedSums(T* inputs);

		/// <summary>
		/// Computes errors for the last layer.
		/// </summary>
		/// <param name="targets">- target values for the outputs;</param>
		inline void computeLastLayerErrors(T* targets);

		/// <summary>
		/// Computes errors for the hidden layers.
		/// </summary>
		/// <param name="nextLayer">- target values for the outputs;</param>
		inline void computeHiddenLayerErrors(NeuralNetworkLayerBase<T>* nextLayer);
		inline float computeMeanSquaredError();

		inline void adjustWeights(T* inputs, float learningRate) {};

	private:

		inline void computeWeightedSums(T* inputs, T* resultsArray) {};
		inline void computeOutputs(T* weightedSumsArray) {};
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	SoftmaxLayer<T>::SoftmaxLayer(size_t outputsCount) :
		NeuralNetworkLayerBase<T>::NeuralNetworkLayerBase(NeuralLayerType::SOFTMAX)
	{
		this->_outputsCount = outputsCount;

		if (this->_outputsCount != 0)
		{
			this->_outputs = new T[this->_outputsCount];
			this->setOutputs(0);
		}
	}

	template <typename T>
	SoftmaxLayer<T>::~SoftmaxLayer()
	{
		delete[] this->_outputs;
	}

	template <typename T>
	inline void SoftmaxLayer<T>::propagate(T* inputs)
	{
		T sum = 0;
		for (size_t i = 0; i < this->_outputsCount; i++)
		{
			this->_outputs[i] = (T)exp(inputs[i]);
			sum += this->_outputs[i];
		}
		for (size_t i = 0; i < this->_outputsCount; i++)
		{
			this->_outputs[i] /= sum;
		}
	}

	template <typename T>
	inline void SoftmaxLayer<T>::propagateSavingWeightedSums(T* inputs)
	{
		propagate(inputs);
	}

	template <typename T>
	inline void SoftmaxLayer<T>::computeLastLayerErrors(T* targets)
	{
		// Exit if targets are not passed.
		if (targets == nullptr)
		{
			return;
		}

		// Exit if errors array is not allocated.
		if (this->_errors == nullptr)
		{
			return;
		}

		for (size_t neuronIndex = 0; neuronIndex < this->_outputsCount; neuronIndex++)
		{
			this->_errors[neuronIndex] = this->_outputs[neuronIndex] - targets[neuronIndex];
		}
	}

	template <typename T>
	inline void SoftmaxLayer<T>::computeHiddenLayerErrors(aimods::NeuralNetworkLayerBase<T>* nextLayer)
	{
		// Exit if nextLayer is not passed.
		if (nextLayer == nullptr)
		{
			return;
		}

		// Exit if errors array is not allocated.
		if (this->_errors == nullptr)
		{
			return;
		}

		FullyConnectedLayer<T>* nextLayerFullyConnected = (FullyConnectedLayer<T>*)nextLayer;
		T* nextLayerWeights = nextLayerFullyConnected->getWeights();
		T* nextLayerErrors = nextLayerFullyConnected->getErrors();

		for (size_t currentLayerNeuronIndex = 0;
			currentLayerNeuronIndex < this->_outputsCount;
			currentLayerNeuronIndex++)
		{
			this->_errors[currentLayerNeuronIndex] = 0;

			for (size_t nextLayerNeuronIndex = 0;
				nextLayerNeuronIndex < nextLayer->getOutputsCount();
				nextLayerNeuronIndex++)
			{
				size_t nextLayerWeightIndex = this->_outputsCount * currentLayerNeuronIndex + nextLayerNeuronIndex;
				this->_errors[currentLayerNeuronIndex] +=
					nextLayerErrors[nextLayerNeuronIndex] * nextLayerWeights[nextLayerWeightIndex];
			}
		}
	}

	template <typename T>
	inline float SoftmaxLayer<T>::computeMeanSquaredError()
	{
		T meanSquaredError = 0;
		for (size_t neuronIndex = 0; neuronIndex < this->_outputsCount; neuronIndex++)
		{
			meanSquaredError += pow(this->_errors[neuronIndex], 2);
		}
		return (float)meanSquaredError / this->_outputsCount;
	}
}

#endif