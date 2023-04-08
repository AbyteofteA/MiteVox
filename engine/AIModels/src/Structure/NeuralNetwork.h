#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/SoftmaxLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2D.h"
#include "engine/AIModels/src/Structure/NeuralLayerCompatibility.h"

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

		std::string name;
		std::string documentation;

		size_t inputsCount;
		size_t outputsCount;
		std::vector<NeuralNetworkLayerBase<T>*> layers;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline NeuralNetwork<T>::NeuralNetwork(std::string name, size_t inputsCount)
	{
		setName(name);
		this->inputsCount = inputsCount;
		outputsCount = inputsCount;
	}

	template <typename T>
	inline NeuralNetwork<T>::~NeuralNetwork()
	{
		name.clear();

		for (NeuralNetworkLayerBase<T>* layer : layers)
		{
			delete layer;
		}

		layers.clear();
	}

	template <typename T>
	inline size_t NeuralNetwork<T>::getInputsCount()
	{
		return inputsCount;
	}

	template <typename T>
	inline size_t NeuralNetwork<T>::getOutputsCount()
	{
		return outputsCount;
	}

	template <typename T>
	inline size_t NeuralNetwork<T>::getLayersCount()
	{
		return layers.size();
	}

	template <typename T>
	inline NeuralNetworkLayerBase<T>* NeuralNetwork<T>::getLayer(size_t layerIndex)
	{
		if (layerIndex < layers.size())
		{
			return layers[layerIndex];
		}
		else
		{
			return nullptr;
		}
	}

	template <typename T>
	inline T* NeuralNetwork<T>::getOutputs()
	{
		NeuralNetworkLayerBase<T>* lastLayer = layers.back();
		return lastLayer->outputs.getElementsArray();
	}

	template <typename T>
	inline std::string NeuralNetwork<T>::getName()
	{
		return name;
	}

	template <typename T>
	inline std::string NeuralNetwork<T>::getDocumentation()
	{
		return documentation;
	}

	template <typename T>
	inline void NeuralNetwork<T>::setName(std::string name)
	{
		this->name = name;
	}

	template <typename T>
	void NeuralNetwork<T>::setDocumentation(std::string documentation)
	{
		this->documentation = documentation;
	}

	template <typename T>
	inline void NeuralNetwork<T>::printOutputs()
	{
		NeuralNetworkLayerBase<T>* lastLayer = layers.back();
		lastLayer->printOutputs();
	}

	template <typename T>
	inline NeuralNetworkLayerBase<T>* NeuralNetwork<T>::addLayer(NeuralNetworkLayerBase<T>* newLayer)
	{
		// Exit if layers are not compatible.
		if (layers.empty() == false)
		{
			NeuralNetworkLayerBase<T>* lastLayer = layers.back();
			if (NeuralLayerCompatibility::isAbleToConnect(lastLayer, newLayer) == false)
			{
				return nullptr;
			}
		}
		else // Update inputs count if it is the first layer.
		{
			inputsCount = newLayer->getInputsCount();
		}

		layers.push_back(newLayer);
		outputsCount = newLayer->getOutputsCount();
		return newLayer;
	}

	template <typename T>
	inline NeuralNetworkLayerBase<T>* NeuralNetwork<T>::addLayerFullyconnected(ActivationFunction function, size_t outputsCount, float dropout)
	{
		FullyConnectedLayer<T>* newFullyConnectedLayer = nullptr;

		if (layers.empty())
		{
			newFullyConnectedLayer = new FullyConnectedLayer<T>(function, inputsCount, outputsCount, dropout);
		}
		else
		{
			NeuralNetworkLayerBase<T>* lastLayer = layers.back();
			NeuralLayerType lastLayerType = lastLayer->getType();
			if (NeuralLayerCompatibility::isAbleToConnectLayerTypes(lastLayerType, NeuralLayerType::FULLYCONNECTED) == false)
			{
				return nullptr;
			}
			size_t neuronCount = lastLayer->getOutputsCount();
			newFullyConnectedLayer = new FullyConnectedLayer<T>(function, neuronCount, outputsCount, dropout);
		}
		newFullyConnectedLayer->setOutputs(0.0);
		newFullyConnectedLayer->setWeightsRandom();
		layers.push_back((NeuralNetworkLayerBase<T>*)newFullyConnectedLayer);
		this->outputsCount = newFullyConnectedLayer->getOutputsCount();
		return (NeuralNetworkLayerBase<T>*)newFullyConnectedLayer;
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

		if (layers.empty())
		{
			newSubsampling2D = new SubsamplingLayer2D<T>(amountOfInputMaps, inputWidth, inputHeight, function, filter);
			inputsCount = newSubsampling2D->getInputsCount();
		}
		else
		{
			NeuralNetworkLayerBase<T>* lastLayer = layers.back();
			NeuralLayerType lastLayerType = lastLayer->getType();
			if (NeuralLayerCompatibility::isAbleToConnectLayerTypes(lastLayerType, NeuralLayerType::SUBSAMPLING_2D) == false)
			{
				return nullptr;
			}
			FilterLayer2DBase<T>* filterLayer2DBase = (FilterLayer2DBase<T>*)lastLayer;
			size_t amountOfOutputMaps = filterLayer2DBase->getAmountOfOutputMaps();
			size_t outputWidth = filterLayer2DBase->getOutputWidth();
			size_t outputHeight = filterLayer2DBase->getOutputHeight();
			newSubsampling2D = new SubsamplingLayer2D<T>(amountOfOutputMaps, outputWidth, outputHeight, function, filter);
		}
		newSubsampling2D->setOutputs(0);
		newSubsampling2D->setWeightsRandom();
		layers.push_back((NeuralNetworkLayerBase<T>*)newSubsampling2D);
		outputsCount = newSubsampling2D->getOutputsCount();
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

		if (layers.empty())
		{
			newConvolutional2D = new ConvolutionalLayer2D<T>(amountOfInputMaps, inputWidth, inputHeight, filter);
			inputsCount = newConvolutional2D->getInputsCount();
		}
		else
		{
			NeuralNetworkLayerBase<T>* lastLayer = layers.back();
			NeuralLayerType lastLayerType = lastLayer->getType();
			if (NeuralLayerCompatibility::isAbleToConnectLayerTypes(lastLayerType, NeuralLayerType::CONVOLUTIONAL_2D) == false)
			{
				return nullptr;
			}
			FilterLayer2DBase<T>* filterLayer2DBase = (FilterLayer2DBase<T>*)lastLayer;
			size_t amountOfOutputMaps = filterLayer2DBase->getAmountOfOutputMaps();
			size_t outputWidth = filterLayer2DBase->getOutputWidth();
			size_t outputHeight = filterLayer2DBase->getOutputHeight();
			newConvolutional2D = new ConvolutionalLayer2D<T>(amountOfOutputMaps, outputWidth, outputHeight, filter);
		}
		newConvolutional2D->setOutputs(0);
		newConvolutional2D->setWeightsRandom();
		layers.push_back((NeuralNetworkLayerBase<T>*)newConvolutional2D);
		outputsCount = newConvolutional2D->getOutputsCount();
		return (NeuralNetworkLayerBase<T>*)newConvolutional2D;
	}

	/*template <typename T>
	static inline SoftmaxLayer<T>* NeuralLayerGeneration::getCompatibleSoftmaxLayer<T>(NeuralNetworkLayerBase<T>* lastLayer)
	{
		NeuralLayerType lastLayerType = lastLayer->getType();
		if (NeuralLayerCompatibility::isAbleToConnectLayerTypes(lastLayerType, NeuralLayerType::SOFTMAX) == false)
		{
			return nullptr;
		}

		size_t neuronCount = lastLayer->getOutputsCount();
		SoftmaxLayer<T>* newSoftmaxLayer = new SoftmaxLayer<T>(neuronCount);
		return newSoftmaxLayer;
	}*/

	template <typename T>
	inline NeuralNetworkLayerBase<T>* NeuralNetwork<T>::removeLastLayer()
	{
		if (layers.empty())
		{
			return nullptr;
		}

		layers.pop_back();
		NeuralNetworkLayerBase<T>* lastLayer = layers.back();
		outputsCount = lastLayer->getOutputsCount();
		return lastLayer;
	}

	template <typename T>
	inline void NeuralNetwork<T>::propagate(T* inputs)
	{
		if (layers.empty())
		{
			return;
		}

		layers[0]->propagate(inputs);
		for (size_t layerIndex = 1; layerIndex < layers.size(); layerIndex++)
		{
			T* previousLayerOutputs = layers[layerIndex - 1]->outputs.getElementsArray();
			layers[layerIndex]->propagate(previousLayerOutputs);
		}
	}

	template <typename T>
	inline void NeuralNetwork<T>::propagateSavingWeightedSums(T* inputs)
	{
		if (layers.empty())
		{
			return;
		}

		layers[0]->propagateSavingWeightedSums(inputs);
		for (size_t layerIndex = 1; layerIndex < layers.size(); layerIndex++)
		{
			T* previousLayerOutputs = layers[layerIndex - 1]->outputs.getElementsArray();
			layers[layerIndex]->propagateSavingWeightedSums(previousLayerOutputs);
		}
	}

	template <typename T>
	inline void NeuralNetwork<T>::computeErrors(T* targets)
	{
		NeuralNetworkLayerBase<T>* lastLayer = layers.back();
		lastLayer->computeLastLayerErrors(targets);

		size_t layersCount = layers.size();
		for (size_t layerIndex = 2; layerIndex <= layersCount; layerIndex++)
		{
			size_t currentLayerIndex = layersCount - layerIndex;
			NeuralNetworkLayerBase<T>* nextLayer = layers[currentLayerIndex + 1];
			layers[currentLayerIndex]->computeHiddenLayerErrors(nextLayer);
		}
	}

	template <typename T>
	inline void NeuralNetwork<T>::adjustWeights(T* inputs, float learningRate)
	{
		if (layers.empty())
		{
			return;
		}

		layers[0]->adjustWeights(inputs, learningRate);
		for (size_t layerIndex = 1; layerIndex < layers.size(); layerIndex++)
		{
			T* previousLayerOutputs = layers[layerIndex - 1]->outputs.getElementsArray();
			layers[layerIndex]->adjustWeights(previousLayerOutputs, learningRate);
		}
	}

	template <typename T>
	inline float NeuralNetwork<T>::computeMeanSquaredError(T* targets)
	{
		if (layers.empty())
		{
			return;
		}

		NeuralNetworkLayerBase<T>* lastLayer = layers.back();
		lastLayer->computeLastLayerErrors(targets);
		return lastLayer->computeMeanSquaredError();
	}

	template <typename T>
	inline void NeuralNetwork<T>::prepareForTraining()
	{
		for (NeuralNetworkLayerBase<T>* layer : layers)
		{
			layer->allocateErrorsArray();
			layer->allocateTemporaryCalculations();
		}
	}

	template <typename T>
	inline void NeuralNetwork<T>::terminateTraining()
	{
		for (NeuralNetworkLayerBase<T>* layer : layers)
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

		NeuralNetworkLayerBase<T>* lastLayer = layers.back();
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
			T* inputSample = inputs + sampleIndex * inputsCount;
			T* outputSample = targets + sampleIndex * outputsCount;

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
		for (size_t outputIndex = 0; outputIndex < outputsCount; outputIndex++)
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
			T* inputSample = inputSamples + sampleIndex * inputsCount;
			T* outputSample = testSamples + sampleIndex * outputsCount;

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
			T* inputSample = inputSamples + sampleIndex * inputsCount;
			T* outputSample = testSamples + sampleIndex * outputsCount;

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