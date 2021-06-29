
#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/SoftmaxLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2D.h"
#include "engine/AIModels/src/Structure/NeuralLayerCompatibility.h"
#include "engine/AIModels/src/Structure/NeuralLayerGeneration.h"

#include <string>
#include <vector>

namespace aimods
{
	template <typename T>
	class NeuralNetwork
	{
	public:

		NeuralNetwork(std::string name, size_t inputsCount);
		~NeuralNetwork();

		inline size_t getInputsCount();
		inline size_t getOutputsCount();
		inline size_t getLayersCount();
		inline NeuralNetworkLayerBase<T>* getLayer(size_t layerIndex);
		inline T* getOutputs();
		inline std::string getName();
		inline std::string getDocumentation();

		inline void setName(std::string name);
		inline void setDocumentation(std::string documentation);

		inline void printOutputs();

		inline NeuralNetworkLayerBase<T>* addLayer(NeuralNetworkLayerBase<T>* layer);
		inline NeuralNetworkLayerBase<T>* addLayerFullyconnected(
			ActivationFunction function, 
			size_t outputsCount, 
			float dropout);
		inline NeuralNetworkLayerBase<T>* addLayerSubsampling2D(
			size_t amountOfInputMaps,
			size_t inputWidth,
			size_t inputHeight,
			ActivationFunction function,
			Filter2D<T>* filter);
		inline NeuralNetworkLayerBase<T>* addLayerConvolutional2D(
			size_t amountOfInputMaps,
			size_t inputWidth,
			size_t inputHeight,
			Filter2D<T>* filter);
		inline NeuralNetworkLayerBase<T>* removeLastLayer();

		inline void propagate(T* inputs);
		inline void propagateSavingWeightedSums(T* inputs);
		inline void computeErrors(T* targets);
		inline void adjustWeights(T* inputs, float learningRate);
		inline float computeMeanSquaredError(T* targets);

		inline void prepareForTraining();
		inline void terminateTraining();
		inline float stepThroughBackpropagation(T* inputs, T* targets, float learningRate);
		
		/// <returns>Current mean square error</returns>
		inline float runBackpropagation(
			T* inputs, T* targets,
			size_t samplesCount,
			float noisePercentage,
			float learningRate, 
			float acceptableError,
			size_t maxIterationsCount);

		inline bool checkClassification(T* inputs, T* targets, float acceptableError);
		inline float validate(T* inputSamples, T* testSamples, size_t batchSize, float acceptableError);
		inline float learnBatch(T* inputSamples, T* testSamples, size_t batchSize, float learningRate);
		inline float runBackpropagationForClassification(
			T* trainingInputs, T* trainingTargets,
			size_t trainingFrom, size_t trainingTo,
			size_t batchSize,
			T* testInputs, T* testTargets, 
			size_t testBatchSize,
			float learningRate,
			float validationPercentage,
			float acceptableError,
			size_t maxIterationsCount);

	private:

		std::string _name;
		std::string _documentation;

		size_t _inputsCount;
		size_t _outputsCount;
		std::vector<NeuralNetworkLayerBase<T>*> _layers;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline NeuralNetwork<T>::NeuralNetwork(std::string name, size_t inputsCount)
	{
		setName(name);
		_inputsCount = inputsCount;
		_outputsCount = inputsCount;
	}

	template <typename T>
	inline NeuralNetwork<T>::~NeuralNetwork()
	{
		_name.clear();

		for (NeuralNetworkLayerBase<T>* layer : _layers)
		{
			delete layer;
		}

		_layers.clear();
	}

	template <typename T>
	inline size_t NeuralNetwork<T>::getInputsCount()
	{
		return _inputsCount;
	}

	template <typename T>
	inline size_t NeuralNetwork<T>::getOutputsCount()
	{
		return _outputsCount;
	}

	template <typename T>
	inline size_t NeuralNetwork<T>::getLayersCount()
	{
		return _layers.size();
	}

	template <typename T>
	inline NeuralNetworkLayerBase<T>* NeuralNetwork<T>::getLayer(size_t layerIndex)
	{
		if (layerIndex < _layers.size())
		{
			return _layers[layerIndex];
		}
		else
		{
			return nullptr;
		}
	}

	template <typename T>
	inline T* NeuralNetwork<T>::getOutputs()
	{
		NeuralNetworkLayerBase<T>* lastLayer = _layers.back();
		return lastLayer->getOutputs();
	}

	template <typename T>
	inline std::string NeuralNetwork<T>::getName()
	{
		return _name;
	}

	template <typename T>
	inline std::string NeuralNetwork<T>::getDocumentation()
	{
		return _documentation;
	}

	template <typename T>
	inline void NeuralNetwork<T>::setName(std::string name)
	{
		_name = name;
	}

	template <typename T>
	void NeuralNetwork<T>::setDocumentation(std::string documentation)
	{
		_documentation = documentation;
	}

	template <typename T>
	inline void NeuralNetwork<T>::printOutputs()
	{
		NeuralNetworkLayerBase<T>* lastLayer = _layers.back();
		lastLayer->printOutputs();
	}

	template <typename T>
	inline NeuralNetworkLayerBase<T>* NeuralNetwork<T>::addLayer(NeuralNetworkLayerBase<T>* newLayer)
	{
		// Exit if layers are not compatible.
		if (_layers.empty() == false)
		{
			NeuralNetworkLayerBase<T>* lastLayer = _layers.back();
			if (NeuralLayerCompatibility::isAbleToConnect(lastLayer, newLayer) == false)
			{
				return nullptr;
			}
		}
		else // Update inputs count if it is the first layer.
		{
			_inputsCount = newLayer->getInputsCount();
		}

		_layers.push_back(newLayer);
		_outputsCount = newLayer->getOutputsCount();
		return newLayer;
	}

	template <typename T>
	inline NeuralNetworkLayerBase<T>* NeuralNetwork<T>::addLayerFullyconnected(ActivationFunction function, size_t outputsCount, float dropout)
	{
		FullyConnectedLayer<T>* newFulluconnected = nullptr;

		if (_layers.empty())
		{
			newFulluconnected = new FullyConnectedLayer<T>(function, _inputsCount, outputsCount, dropout);
		}
		else
		{
			NeuralNetworkLayerBase<T>* lastLayer = _layers.back();

			newFulluconnected = NeuralLayerGeneration::getCompatibleFullyConnectedLayer<T>(
				lastLayer, function, outputsCount, dropout);
		}

		if (newFulluconnected != nullptr)
		{
			newFulluconnected->setOutputs(0);
			newFulluconnected->setWeights(0.001);
			_layers.push_back((NeuralNetworkLayerBase<T>*)newFulluconnected);
			_outputsCount = newFulluconnected->getOutputsCount();
		}

		return (NeuralNetworkLayerBase<T>*)newFulluconnected;
	}

	template <typename T>
	inline NeuralNetworkLayerBase<T>* NeuralNetwork<T>::addLayerSubsampling2D(
		size_t amountOfInputMaps, 
		size_t inputWidth, 
		size_t inputHeight, 
		ActivationFunction function,
		Filter2D<T>* filter)
	{
		SubsamplingLayer2D<T>* newSubsampling2D = nullptr;

		if (_layers.empty())
		{
			newSubsampling2D = new SubsamplingLayer2D<T>(amountOfInputMaps, inputWidth, inputHeight, function, filter);
			_inputsCount = newSubsampling2D->getInputsCount();
		}
		else
		{
			NeuralNetworkLayerBase<T>* lastLayer = _layers.back();
			newSubsampling2D = NeuralLayerGeneration::getCompatibleSubsamplingLayer2D<T>(lastLayer, function, filter);
		}

		if (newSubsampling2D != nullptr)
		{
			newSubsampling2D->setOutputs(0);
			newSubsampling2D->setWeights(0.001);
			_layers.push_back((NeuralNetworkLayerBase<T>*)newSubsampling2D);
			_outputsCount = newSubsampling2D->getOutputsCount();
		}

		return (NeuralNetworkLayerBase<T>*)newSubsampling2D;
	}

	template <typename T>
	inline NeuralNetworkLayerBase<T>* NeuralNetwork<T>::addLayerConvolutional2D(
		size_t amountOfInputMaps,
		size_t inputWidth,
		size_t inputHeight,
		Filter2D<T>* filter)
	{
		ConvolutionalLayer2D<T>* newConvolutional2D = nullptr;

		if (_layers.empty())
		{
			newConvolutional2D = new ConvolutionalLayer2D<T>(amountOfInputMaps, inputWidth, inputHeight, filter);
			_inputsCount = newConvolutional2D->getInputsCount();
		}
		else
		{
			NeuralNetworkLayerBase<T>* lastLayer = _layers.back();
			newConvolutional2D = NeuralLayerGeneration::getCompatibleConvolutionalLayer2D<T>(lastLayer, filter);
		}

		if (newConvolutional2D != nullptr)
		{
			newConvolutional2D->setOutputs(0);
			newConvolutional2D->setWeights(0.001);
			_layers.push_back((NeuralNetworkLayerBase<T>*)newConvolutional2D);
			_outputsCount = newConvolutional2D->getOutputsCount();
		}

		return (NeuralNetworkLayerBase<T>*)newConvolutional2D;
	}

	template <typename T>
	inline NeuralNetworkLayerBase<T>* NeuralNetwork<T>::removeLastLayer()
	{
		if (_layers.empty())
		{
			return nullptr;
		}

		_layers.pop_back();
		NeuralNetworkLayerBase<T>* lastLayer = _layers.back();
		_outputsCount = lastLayer->getOutputsCount();
		return lastLayer;
	}

	template <typename T>
	inline void NeuralNetwork<T>::propagate(T* inputs)
	{
		_layers[0]->propagate(inputs);
		for (size_t layerIndex = 1; layerIndex < _layers.size(); layerIndex++)
		{
			T* previousLayerOutputs = _layers[layerIndex - 1]->getOutputs();
			_layers[layerIndex]->propagate(previousLayerOutputs);
		}
	}

	template <typename T>
	inline void NeuralNetwork<T>::propagateSavingWeightedSums(T* inputs)
	{
		_layers[0]->propagateSavingWeightedSums(inputs);
		for (size_t layerIndex = 1; layerIndex < _layers.size(); layerIndex++)
		{
			T* previousLayerOutputs = _layers[layerIndex - 1]->getOutputs();
			_layers[layerIndex]->propagateSavingWeightedSums(previousLayerOutputs);
		}
	}

	template <typename T>
	inline void NeuralNetwork<T>::computeErrors(T* targets)
	{
		NeuralNetworkLayerBase<T>* lastLayer = _layers.back();
		lastLayer->computeLastLayerErrors(targets);

		size_t layersCount = _layers.size();
		for (size_t layerIndex = 2; layerIndex <= layersCount; layerIndex++)
		{
			size_t currentLayerIndex = layersCount - layerIndex;
			NeuralNetworkLayerBase<T>* nextLayer = _layers[currentLayerIndex + 1];
			_layers[currentLayerIndex]->computeHiddenLayerErrors(nextLayer);
		}
	}

	template <typename T>
	inline void NeuralNetwork<T>::adjustWeights(T* inputs, float learningRate)
	{
		_layers[0]->adjustWeights(inputs, learningRate);
		for (size_t layerIndex = 1; layerIndex < _layers.size(); layerIndex++)
		{
			NeuralNetworkLayerBase<T>* previousLayer = _layers[layerIndex - 1];
			_layers[layerIndex]->adjustWeights(previousLayer->getOutputs(), learningRate);
		}
	}

	template <typename T>
	inline float NeuralNetwork<T>::computeMeanSquaredError(T* targets)
	{
		NeuralNetworkLayerBase<T>* lastLayer = _layers.back();
		lastLayer->computeLastLayerErrors(targets);
		return lastLayer->computeMeanSquaredError();
	}

	template <typename T>
	inline void NeuralNetwork<T>::prepareForTraining()
	{
		for (NeuralNetworkLayerBase<T>* layer : _layers)
		{
			layer->allocateErrorsArray();
			layer->allocateTemporaryCalculations();
		}
	}

	template <typename T>
	inline void NeuralNetwork<T>::terminateTraining()
	{
		for (NeuralNetworkLayerBase<T>* layer : _layers)
		{
			layer->freeErrorsArray();
			layer->freeTemporaryCalculations();
		}
	}

	template <typename T>
	inline float NeuralNetwork<T>::stepThroughBackpropagation(T* inputs, T* targets, float learningRate)
	{
		propagateSavingWeightedSums(inputs);
		computeErrors(targets);
		adjustWeights(inputs, learningRate);

		NeuralNetworkLayerBase<T>* lastLayer = _layers.back();
		return lastLayer->computeMeanSquaredError();
	}

	template <typename T>
	inline float NeuralNetwork<T>::runBackpropagation(
		T* inputs, T* targets,
		size_t samplesCount,
		float noisePercentage,
		float learningRate,
		float acceptableError,
		size_t maxIterationsCount)
	{
		float error = 9999998.0f;
		float prevError = 9999999.0f;
		size_t sampleIndex = 0;
		size_t iteration = 0;
		T* inputSample = nullptr;
		T* outputSample = nullptr;

		while (error > acceptableError)
		{
			T* inputSample = inputs + sampleIndex * _inputsCount;
			T* outputSample = targets + sampleIndex * _outputsCount;

			prevError = error;
			error = stepThroughBackpropagation(inputSample, outputSample, learningRate);

			if (prevError < error)
			{
				break;
			}
			iteration++;
			if (iteration >= maxIterationsCount)
			{
				break;
			}
			sampleIndex++;
			if (sampleIndex >= samplesCount)
			{
				sampleIndex = 0;
			}
		}
		return error;
	}

	template <typename T>
	inline bool NeuralNetwork<T>::checkClassification(T* inputs, T* targets, float acceptableError)
	{
		propagate(inputs);

		T* outputs = this->getOutputs();
		for (size_t outputIndex = 0; outputIndex < _outputsCount; outputIndex++)
		{
			if (abs(outputs[outputIndex] - targets[outputIndex]) > acceptableError)
			{
				return false;
			}
		}
		
		return true;
	}

	template <typename T>
	inline float NeuralNetwork<T>::validate(
		T* inputSamples, T* testSamples, size_t batchSize, float acceptableError)
	{
		size_t rightAnswers = 0;
		size_t samplesCount = 0;
		for (size_t sampleIndex = 0; sampleIndex < batchSize; sampleIndex++)
		{
			T* inputSample = inputSamples + sampleIndex * _inputsCount;
			T* outputSample = testSamples + sampleIndex * _outputsCount;

			if (checkClassification(inputSample, outputSample, acceptableError))
			{
				rightAnswers += 1;
			}
			samplesCount += 1;
		}
		return (float)rightAnswers / (float)samplesCount;
	}

	template <typename T>
	inline float NeuralNetwork<T>::learnBatch(
		T* inputSamples, T* testSamples, size_t batchSize, float learningRate)
	{
		float error = 0.0f;
		for (size_t sampleIndex = 0; sampleIndex < batchSize; sampleIndex++)
		{
			T* inputSample = inputSamples + sampleIndex * _inputsCount;
			T* outputSample = testSamples + sampleIndex * _outputsCount;

			error += stepThroughBackpropagation(inputSample, outputSample, learningRate);
		}
		return error / batchSize;
	}

	template <typename T>
	inline float NeuralNetwork<T>::runBackpropagationForClassification(
		T* trainingInputs, T* trainingTargets,
		size_t trainingFrom, size_t trainingTo,
		size_t batchSize,
		T* testInputs, T* testTargets, 
		size_t testBatchSize,
		float learningRate,
		float validationPercentage,
		float acceptableError,
		size_t maxIterationsCount)
	{
		float precesion = 0.0f;
		size_t batchIndex = 0;
		size_t iteration = 0;

		while ((precesion >= validationPercentage) ||
			   (iteration < maxIterationsCount))
		{
			T* trainingInputSample = trainingInputs + batchIndex * batchSize;
			T* trainingOutputSample = trainingTargets + batchIndex * batchSize;
			float error = learnBatch(trainingInputSample, trainingOutputSample, batchSize, learningRate);

			precesion = validate(testInputs, testTargets, testBatchSize, acceptableError);
			iteration++;
		}
		return precesion;
	}
}

#endif