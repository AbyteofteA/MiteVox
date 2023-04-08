#ifndef SOFTMAXLAYER_H
#define SOFTMAXLAYER_H

#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"

#include <cmath>

namespace aimods
{
	template <typename T>
	class SoftmaxLayer : public NeuralNetworkLayerBase<T> // TODO: : public FullyConnectedLayer<T>
	{
	public:

		SoftmaxLayer(size_t outputsCount);
		~SoftmaxLayer();

		inline void setWeightsRandom() {};
		inline void setWeights(T value) {};
		inline T* getWeights() { return nullptr; };
		inline size_t getWeightsCount() { return 0; };
		inline T* getThresholds() { return nullptr; };

		/// <summary>
		/// Calculates Softmax of inputs, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "size"</param>
		inline void propagate(safety::SafeArray<T>* inputs);
		inline void propagateSavingWeightedSums(safety::SafeArray<T>* inputs);

		inline void computeHiddenLayerErrors(NeuralNetworkLayerBase<T>* nextLayer);
		inline float computeMeanSquaredError();

		inline void adjustWeights(safety::SafeArray<T>* inputs, float learningRate) {};

	private:

		inline void computeWeightedSums(safety::SafeArray<T>* inputs, safety::SafeArray<T>* resultsArray) {};
		inline void computeOutputs(safety::SafeArray<T>* weightedSumsArray) {};
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	SoftmaxLayer<T>::SoftmaxLayer(size_t outputsCount) :
		NeuralNetworkLayerBase<T>::NeuralNetworkLayerBase(NeuralLayerType::SOFTMAX)
	{
		if (outputsCount != 0)
		{
			this->outputs.resize(outputsCount);
			this->outputs.setAllElements(0);
		}
	}

	template <typename T>
	SoftmaxLayer<T>::~SoftmaxLayer()
	{
		this->outputs.deallocate();
	}

	template <typename T>
	inline void SoftmaxLayer<T>::propagate(safety::SafeArray<T>* inputs)
	{
		T sum = 0.0;
		size_t outputsCount = this->outputs.getElementsCount();
		for (size_t i = 0; i < outputsCount; i++)
		{
			this->outputs[i] = (T)std::exp(inputs->getElement(i));
			sum += this->outputs[i];
		}
		for (size_t i = 0; i < outputsCount; i++)
		{
			this->outputs[i] /= sum;
		}
	}

	template <typename T>
	inline void SoftmaxLayer<T>::propagateSavingWeightedSums(safety::SafeArray<T>* inputs)
	{
		propagate(inputs);
	}

	template <typename T>
	inline void SoftmaxLayer<T>::computeHiddenLayerErrors(aimods::NeuralNetworkLayerBase<T>* nextLayer)
	{
		if (nextLayer == nullptr ||
			this->errors.getElementsCount() == 0)
		{
			return;
		}

		FullyConnectedLayer<T>* nextLayerFullyConnected = (FullyConnectedLayer<T>*)nextLayer;
		T* nextLayerWeights = nextLayerFullyConnected->getWeights();
		T* nextLayerErrors = nextLayerFullyConnected->errors.getElementsArray();

		size_t outputsCount = this->outputs.getElementsCount();
		for (size_t currentLayerNeuronIndex = 0;
			currentLayerNeuronIndex < outputsCount;
			currentLayerNeuronIndex++)
		{
			this->errors[currentLayerNeuronIndex] = 0;

			for (size_t nextLayerNeuronIndex = 0;
				nextLayerNeuronIndex < nextLayer->getOutputsCount();
				nextLayerNeuronIndex++)
			{
				size_t nextLayerWeightIndex = outputsCount * currentLayerNeuronIndex + nextLayerNeuronIndex;
				this->errors[currentLayerNeuronIndex] +=
					nextLayerErrors[nextLayerNeuronIndex] * nextLayerWeights[nextLayerWeightIndex];
			}
		}
	}

	template <typename T>
	inline float SoftmaxLayer<T>::computeMeanSquaredError()
	{
		T meanSquaredError = 0;
		size_t outputsCount = this->outputs.getElementsCount();
		for (size_t neuronIndex = 0; neuronIndex < outputsCount; neuronIndex++)
		{
			meanSquaredError += pow(this->errors[neuronIndex], 2);
		}
		return (float)meanSquaredError / outputsCount;
	}
}

#endif