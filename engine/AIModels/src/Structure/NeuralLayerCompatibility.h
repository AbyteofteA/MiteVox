
#ifndef NEURALLAYERCOMPATIBILITY_H
#define NEURALLAYERCOMPATIBILITY_H

#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/SoftmaxLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2D.h"

namespace aimods
{
	namespace NeuralLayerCompatibility
	{
		inline bool isAbleToConnectLayerTypes(
			NeuralLayerType neuralLayerType1, 
			NeuralLayerType neuralLayerType2);

		template <typename T>
		inline bool isAbleToConnect(
			NeuralNetworkLayerBase<T>* layer1, 
			NeuralNetworkLayerBase<T>* layer2);
	};


	// IMPLEMENTATION BELOW //


	inline bool NeuralLayerCompatibility::isAbleToConnectLayerTypes(
		NeuralLayerType neuralLayerType1, 
		NeuralLayerType neuralLayerType2)
	{
		// This conditions reacts only on cases when able to connect.
		if (neuralLayerType1 == NeuralLayerType::FULLYCONNECTED)
		{
			if (neuralLayerType2 == NeuralLayerType::FULLYCONNECTED ||
				neuralLayerType2 == NeuralLayerType::SOFTMAX ||
				neuralLayerType2 == NeuralLayerType::HOPFIELD)
			{
				return true;
			}
		}
		else if (
			neuralLayerType1 == NeuralLayerType::CONVOLUTIONAL_2D ||
			neuralLayerType1 == NeuralLayerType::DECONVOLUTIONAL_2D || 
			neuralLayerType1 == NeuralLayerType::SUBSAMPLING_2D || 
			neuralLayerType1 == NeuralLayerType::UPSAMPLING_2D ||
			neuralLayerType1 == NeuralLayerType::CONVOLUTIONAL_1X1 ||
			neuralLayerType1 == NeuralLayerType::CONVOLUTIONAL_DEPTHWISE_SEPARABLE_2D || 
			neuralLayerType1 == NeuralLayerType::CONVOLUTIONAL_SPATIALLY_SEPARABLE_2D ||
			neuralLayerType1 == NeuralLayerType::CONVOLUTIONAL_DIALATED_2D)
		{
			if (neuralLayerType2 != NeuralLayerType::INVALID &&
				neuralLayerType2 != NeuralLayerType::HOPFIELD)
			{
				return true;
			}
		}

		// In other cases layer types cannot be connected.
		return false;
	}

	template <typename T>
	inline bool NeuralLayerCompatibility::isAbleToConnect(
		NeuralNetworkLayerBase<T>* layer1, 
		NeuralNetworkLayerBase<T>* layer2)
	{
		// If layer1 doesn't exist, we can connect anything.
		if (layer1 == nullptr)
		{
			return true;
		}

		// Exit if types are not compatible.
		NeuralLayerType layerType1 = layer1->getType();
		NeuralLayerType layerType2 = layer2->getType();
		if (NeuralLayerCompatibility::isAbleToConnectLayerTypes(layerType1, layerType2) == false)
		{
			return false;
		}

		// Exit if inputs and outputs are not compatible.
		size_t outputsCount = layer1->getOutputsCount();
		size_t inputsCount = layer2->getInputsCount();
		if (outputsCount != inputsCount)
		{
			return false;
		}

		// TODO: check for special cases here.

		return true;
	}
}

#endif