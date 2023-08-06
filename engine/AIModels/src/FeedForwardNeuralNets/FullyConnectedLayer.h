#ifndef FULLYCONNECTEDLAYER_H
#define FULLYCONNECTEDLAYER_H

#include "engine/AIModels/src/Structure/Functions.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/CodeSafety/src/ensureValue.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <cstdlib>

namespace aimods
{
	/// <summary>
	/// 
	/// </summary>
	/// TODO: Introduce randomness into  setWeightsRandom().
	/// 
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class FullyConnectedLayer : public NeuralNetworkLayerBase<T>
	{
	public:

		safety::SafeArray<T> thresholds;

		/// <summary>
		/// Weights are stored like this:
		/// [firstNeuronToPrevLayerWeights], [secondNeuronToPrevLayerWeights],
		/// </summary>
		safety::SafeArray<T> weights;

		FullyConnectedLayer(ActivationFunction function, size_t inputsCount, size_t outputsCount, float dropout);
		~FullyConnectedLayer();

		inline void setNeuronWeights(size_t neuronIndex, T value);
		inline void setWeightsRandom();
		inline void setWeights(T value);
		inline T* getWeights();
		inline size_t getWeightsCount();
		inline T* getThresholds();
		
		inline float getDropout();

		inline void computeHiddenLayerErrors(NeuralNetworkLayerBase<T>* nextLayer);

		/// <summary>
		/// Propagates signals through the layer, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "prevSize"</param>
		inline void propagate(safety::SafeArray<T>* inputs);
		inline void propagateSavingWeightedSums(safety::SafeArray<T>* inputs);

		inline void adjustWeights(safety::SafeArray<T>* inputs, float learningRate);

	private:

		float dropout;

		/// <summary>
		/// Calculates weighted sum, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "prevSize"</param>
		inline void computeWeightedSums(safety::SafeArray<T>* inputs, safety::SafeArray<T>* resultsArray);

		/// <summary>
		/// Applies activation function to weighted sum stored in outputs (therefore
		/// weighSum(inputs) must be called first), then stores the results in outputs.
		/// </summary>
		inline void computeOutputs(safety::SafeArray<T>* weightedSumsArray);
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	FullyConnectedLayer<T>::FullyConnectedLayer(ActivationFunction function, size_t inputsCount, size_t outputsCount, float dropout) :
		NeuralNetworkLayerBase<T>::NeuralNetworkLayerBase(NeuralLayerType::FULLYCONNECTED)
	{
		this->dropout = safety::ensureRange<float>(dropout, 0.0f, 100.0f);
		this->inputsCount = inputsCount;
		this->function = function;

		if (outputsCount == 0)
		{
			return;
		}
		this->outputs.resize(outputsCount);
		this->outputs.setAllElements(0.0f);
		thresholds.resize(outputsCount);
		thresholds.setAllElements(0.0f);

		if (this->inputsCount == 0)
		{
			return;
		}
		size_t weightsCount = inputsCount * outputsCount;
		weights.resize(weightsCount);
		weights.setAllElements(0.0f);
	}

	template <typename T>
	FullyConnectedLayer<T>::~FullyConnectedLayer()
	{
		weights.deallocate();
		this->outputs.deallocate();
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::setNeuronWeights(size_t neuronIndex, T value)
	{
		size_t weightsOffset = neuronIndex * this->inputsCount;
		for (size_t weightIndex = 0; weightIndex < this->inputsCount; weightIndex++)
		{
			weights[weightsOffset + weightIndex] = value;
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::setWeightsRandom()
	{
		size_t weightsCount = weights.getElementsCount();
		for (size_t weightIndex = 0; weightIndex < weightsCount; weightIndex++)
		{
			weights[weightIndex] = (T)rand() / ((T)RAND_MAX * 10.0f);
		}
		size_t thresholdsCount = thresholds.getElementsCount();
		for (size_t thresholdIndex = 0; thresholdIndex < thresholdsCount; thresholdIndex++)
		{
			thresholds[thresholdIndex] = (T)rand() / ((T)RAND_MAX * 10.0f);
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::setWeights(T value)
	{
		weights.setAllElements(value);
	}

	template <typename T>
	inline T* FullyConnectedLayer<T>::getWeights()
	{
		return weights.getElementsArray();
	}

	template <typename T>
	inline size_t FullyConnectedLayer<T>::getWeightsCount()
	{
		return weights.getElementsCount();
	}

	template <typename T>
	inline T* FullyConnectedLayer<T>::getThresholds()
	{
		return thresholds.getElementsArray();
	}

	template <typename T>
	inline float FullyConnectedLayer<T>::getDropout()
	{
		return dropout;
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::computeHiddenLayerErrors(aimods::NeuralNetworkLayerBase<T>* nextLayer)
	{
		if (nextLayer == nullptr ||
			this->isReadyToBeTrained() == false)
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
	inline void FullyConnectedLayer<T>::propagate(safety::SafeArray<T>* inputs)
	{
		computeWeightedSums(inputs, &this->outputs);
		computeOutputs(&this->outputs);
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::propagateSavingWeightedSums(safety::SafeArray<T>* inputs)
	{
		computeWeightedSums(inputs, &this->temporaryCalculations);
		computeOutputs(&this->temporaryCalculations);
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::adjustWeights(safety::SafeArray<T>* inputs, float learningRate)
	{
		if (inputs->getElementsCount() == 0)
		{
			return;
		}

		size_t outputsCount = this->outputs.getElementsCount();
		for (size_t currentLayerNeuronIndex = 0;
			currentLayerNeuronIndex < outputsCount;
			currentLayerNeuronIndex++)
		{
			size_t weightsOffset = currentLayerNeuronIndex * this->inputsCount;

			for (size_t previousLayerNeuronIndex = 0;
				previousLayerNeuronIndex < this->inputsCount;
				previousLayerNeuronIndex++)
			{
				T deltaWeight = 
					learningRate * this->errors[currentLayerNeuronIndex] * 
					activationDerivative<T>(this->temporaryCalculations[currentLayerNeuronIndex], this->function) *
					inputs->getElement(previousLayerNeuronIndex);

				this->weights[previousLayerNeuronIndex + weightsOffset] -= deltaWeight;
			}

			T deltaThreshold =
				learningRate * this->errors[currentLayerNeuronIndex] *
				activationDerivative<T>(this->temporaryCalculations[currentLayerNeuronIndex], this->function);

			this->thresholds[currentLayerNeuronIndex] += deltaThreshold;
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::computeWeightedSums(safety::SafeArray<T>* inputs, safety::SafeArray<T>* resultsArray)
	{
		if (inputs->getElementsCount() == 0 ||
			resultsArray->getElementsCount() == 0)
		{
			return;
		}

		size_t outputsCount = this->outputs.getElementsCount();
		for (size_t currentLayerNeuronIndex = 0;
			currentLayerNeuronIndex < outputsCount;
			currentLayerNeuronIndex++)
		{
			T sum = 0;
			size_t weightsOffset = currentLayerNeuronIndex * this->inputsCount;

			for (size_t previousLayerNeuronIndex = 0;
				previousLayerNeuronIndex < this->inputsCount;
				previousLayerNeuronIndex++)
			{
				sum += inputs->getElement(previousLayerNeuronIndex) * weights[previousLayerNeuronIndex + weightsOffset];
			}
			//sum -= thresholds[currentLayerNeuronIndex];
			resultsArray->setElement(currentLayerNeuronIndex, sum);
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::computeOutputs(safety::SafeArray<T>* weightedSumsArray)
	{
		if (weightedSumsArray->getElementsCount())
		{
			return;
		}

		size_t outputsCount = this->outputs.getElementsCount();
		for (size_t neuronIndex = 0; neuronIndex < outputsCount; neuronIndex++)
		{
			this->outputs[neuronIndex] =
				activationFunction<T>(weightedSumsArray->getElement(neuronIndex), this->function);
		}
	}
}

#endif
