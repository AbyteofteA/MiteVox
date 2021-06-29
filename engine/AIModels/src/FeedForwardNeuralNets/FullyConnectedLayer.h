
#ifndef FULLYCONNECTEDLAYER_H
#define FULLYCONNECTEDLAYER_H

#include "engine/AIModels/src/Structure/Functions.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/CodeSafety/src/ensureValue.h"

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

		FullyConnectedLayer(ActivationFunction function, size_t inputsCount, size_t outputsCount, float dropout);
		~FullyConnectedLayer();

		inline void setNeuronWeights(size_t neuronIndex, T value);
		inline void setWeightsRandom();
		inline void setWeights(T value);
		inline T* getWeights();
		inline size_t getWeightsCount();
		inline T* getThresholds();
		
		inline float getDropout();

		/// <summary>
		/// Computes errors for the hidden layers.
		/// </summary>
		/// <param name="nextLayer">- target values for the outputs;</param>
		inline void computeHiddenLayerErrors(NeuralNetworkLayerBase<T>* nextLayer);

		/// <summary>
		/// Propagates signals through the layer, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "prevSize"</param>
		inline void propagate(T* inputs);
		inline void propagateSavingWeightedSums(T* inputs);

		inline void adjustWeights(T* inputs, float learningRate);

	private:

		T* _thresholds;
		size_t _weightsCount;
		T* _weights;			///< Weights are stored like this:
								///< [firstNeuronToPrevLayerWeights], [secondNeuronToPrevLayerWeights], ...
		float _dropout;

		/// <summary>
		/// Calculates weighted sum, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "prevSize"</param>
		inline void computeWeightedSums(T* inputs, T* resultsArray);

		/// <summary>
		/// Applies activation function to weighted sum stored in outputs (therefore
		/// weighSum(inputs) must be called first), then stores the results in outputs.
		/// </summary>
		inline void computeOutputs(T* weightedSumsArray);
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	FullyConnectedLayer<T>::FullyConnectedLayer(ActivationFunction function, size_t inputsCount, size_t outputsCount, float dropout) :
		NeuralNetworkLayerBase<T>::NeuralNetworkLayerBase(NeuralLayerType::FULLYCONNECTED)
	{
		_thresholds = nullptr;
		_weights = nullptr;
		_dropout = safety::ensureRange<float>(dropout, 0.0, 100.0);

		this->_outputsCount = outputsCount;
		this->_inputsCount = inputsCount;
		_weightsCount = this->_inputsCount * this->_outputsCount;
		this->_function = function;

		if (this->_outputsCount != 0)
		{
			this->_outputs = new T[this->_outputsCount];
			_thresholds = new T[this->_outputsCount];

			size_t outputsSize = this->_outputsCount * sizeof(T);
			memset(this->_outputs, 0, outputsSize);
			memset(_thresholds, 0, outputsSize);

			if (this->_inputsCount != 0)
			{
				_weights = new T[_weightsCount];
				memset(_weights, 0, _weightsCount * sizeof(T));
			}
			else
			{
				_weights = nullptr;
			}
		}
	}

	template <typename T>
	FullyConnectedLayer<T>::~FullyConnectedLayer()
	{
		delete[] _weights;
		delete[] this->_outputs;
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::setNeuronWeights(size_t neuronIndex, T value)
	{
		size_t weightsOffset = neuronIndex * this->_inputsCount;
		for (size_t weightIndex = 0; weightIndex < this->_inputsCount; weightIndex++)
		{
			_weights[weightsOffset + weightIndex] = value;
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::setWeightsRandom()
	{
		for (size_t weightIndex = 0; weightIndex < _weightsCount; weightIndex++)
		{
			_weights[weightIndex] = 0;
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::setWeights(T value)
	{
		for (size_t weightIndex = 0; weightIndex < _weightsCount; weightIndex++)
		{
			_weights[weightIndex] = value;
		}
	}

	template <typename T>
	inline T* FullyConnectedLayer<T>::getWeights()
	{
		return _weights;
	}

	template <typename T>
	inline size_t FullyConnectedLayer<T>::getWeightsCount()
	{
		return _weightsCount;
	}

	template <typename T>
	inline T* FullyConnectedLayer<T>::getThresholds()
	{
		return _thresholds;
	}

	template <typename T>
	inline float FullyConnectedLayer<T>::getDropout()
	{
		return _dropout;
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::computeHiddenLayerErrors(aimods::NeuralNetworkLayerBase<T>* nextLayer)
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
	inline void FullyConnectedLayer<T>::propagate(T* inputs)
	{
		computeWeightedSums(inputs, this->_outputs);
		computeOutputs(this->_outputs);
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::propagateSavingWeightedSums(T* inputs)
	{
		computeWeightedSums(inputs, this->_temporaryCalculations);
		computeOutputs(this->_temporaryCalculations);
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::adjustWeights(T* inputs, float learningRate)
	{
		// Exit if inputs are not passed.
		if (inputs == nullptr)
		{
			return;
		}

		for (size_t currentLayerNeuronIndex = 0;
			currentLayerNeuronIndex < this->_outputsCount;
			currentLayerNeuronIndex++)
		{
			size_t weightsOffset = currentLayerNeuronIndex * this->_inputsCount;

			for (size_t previousLayerNeuronIndex = 0;
				previousLayerNeuronIndex < this->_inputsCount;
				previousLayerNeuronIndex++)
			{
				T deltaWeight = 
					learningRate * this->_errors[currentLayerNeuronIndex] * 
					activationDerivative<T>(this->_temporaryCalculations[currentLayerNeuronIndex], this->_function) *
					inputs[previousLayerNeuronIndex];

				this->_weights[previousLayerNeuronIndex + weightsOffset] -= deltaWeight;
			}

			T deltaThreshold =
				learningRate * this->_errors[currentLayerNeuronIndex] *
				activationDerivative<T>(this->_temporaryCalculations[currentLayerNeuronIndex], this->_function);

			this->_thresholds[currentLayerNeuronIndex] += deltaThreshold;
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::computeWeightedSums(T* inputs, T* resultsArray)
	{
		// Exit if inputs are not passed.
		if (inputs == nullptr)
		{
			return;
		}

		// Exit if resultsArray for weighted sums is not passed.
		if (resultsArray == nullptr)
		{
			return;
		}

		for (size_t currentLayerNeuronIndex = 0;
			currentLayerNeuronIndex < this->_outputsCount;
			currentLayerNeuronIndex++)
		{
			T sum = 0;
			size_t weightsOffset = currentLayerNeuronIndex * this->_inputsCount;

			for (size_t previousLayerNeuronIndex = 0;
				previousLayerNeuronIndex < this->_inputsCount;
				previousLayerNeuronIndex++)
			{
				sum += inputs[previousLayerNeuronIndex] * _weights[previousLayerNeuronIndex + weightsOffset];
			}
			//sum -= _thresholds[currentLayerNeuronIndex];
			resultsArray[currentLayerNeuronIndex] = sum;
		}
	}

	template <typename T>
	inline void FullyConnectedLayer<T>::computeOutputs(T* weightedSumsArray)
	{
		// Exit if weightedSumsArray is not passed.
		if (weightedSumsArray == nullptr)
		{
			return;
		}

		for (size_t neuronIndex = 0; neuronIndex < this->_outputsCount; neuronIndex++)
		{
			this->_outputs[neuronIndex] =
				activationFunction<T>(weightedSumsArray[neuronIndex], this->_function);
		}
	}
}

#endif
