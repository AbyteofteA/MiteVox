
#ifndef NEURALLAYERGENERATOR_H
#define NEURALLAYERGENERATOR_H

#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/SoftmaxLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2D.h"
#include "NeuralLayerCompatibility.h"

namespace aimods
{
	namespace NeuralLayerGeneration
	{
		// Methods that generate compatible SubsamplingLayer2D 
		// given previous layer and subsampling filter.

		template <typename T>
		static inline SubsamplingLayer2D<T>* getCompatibleSubsamplingLayer2D(
			NeuralNetworkLayerBase<T>* lastLayer,
			ActivationFunction function,
			Filter2D<T>* filter);
		
		// Methods that generate compatible ConvolutionalLayer2D 
		// given previous layer and convolutional filter.

		template <typename T>
		static inline ConvolutionalLayer2D<T>* getCompatibleConvolutionalLayer2D(
			NeuralNetworkLayerBase<T>* lastLayer,
			Filter2D<T>* filter);

		/// <summary>
		/// Methods that generate compatible FullyConnectedLayer 
		/// given previous layer, activation function and amount of neurons.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="lastLayer"></param>
		/// <param name="function"></param>
		/// <param name="newNeuronCount"></param>
		/// <param name="dropout"></param>
		/// <returns></returns>
		template <typename T>
		static inline FullyConnectedLayer<T>* getCompatibleFullyConnectedLayer(
			NeuralNetworkLayerBase<T>* lastLayer,
			ActivationFunction function,
			size_t newNeuronCount, 
			float dropout);

		/// <summary>
		/// Method that generates compatible SoftmaxLayer given previous layer.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="lastLayer"></param>
		/// <returns></returns>
		template <typename T>
		static inline SoftmaxLayer<T>* getCompatibleSoftmaxLayer(NeuralNetworkLayerBase<T>* lastLayer);
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	static inline SubsamplingLayer2D<T>* NeuralLayerGeneration::getCompatibleSubsamplingLayer2D<T>(
		NeuralNetworkLayerBase<T>* lastLayer,
		ActivationFunction function,
		Filter2D<T>* filter)
	{
		NeuralLayerType lastLayerType = lastLayer->getType();
		if (NeuralLayerCompatibility::isAbleToConnectLayerTypes(lastLayerType, NeuralLayerType::SUBSAMPLING_2D) == false)
		{
			return nullptr;
		}

		FilterLayer2DBase<T>* filterLayer2DBase = (FilterLayer2DBase<T>*)lastLayer;
		size_t amountOfOutputMaps = filterLayer2DBase->getAmountOfOutputMaps();
		size_t outputWidth = filterLayer2DBase->getOutputWidth();
		size_t outputHeight = filterLayer2DBase->getOutputHeight();

		SubsamplingLayer2D<T>* subsamplingLayer2D = new SubsamplingLayer2D<T>(
			amountOfOutputMaps, outputWidth, outputHeight, function, filter);

		return nullptr;
	}
	
	template <typename T>
	static inline ConvolutionalLayer2D<T>* NeuralLayerGeneration::getCompatibleConvolutionalLayer2D<T>(
		NeuralNetworkLayerBase<T>* lastLayer,
		Filter2D<T>* filter)
	{
		NeuralLayerType lastLayerType = lastLayer->getType();
		if (NeuralLayerCompatibility::isAbleToConnectLayerTypes(lastLayerType, NeuralLayerType::CONVOLUTIONAL_2D) == false)
		{
			return nullptr;
		}

		FilterLayer2DBase<T>* filterLayer2DBase = (FilterLayer2DBase<T>*)lastLayer;
		size_t amountOfOutputMaps = filterLayer2DBase->getAmountOfOutputMaps();
		size_t outputWidth = filterLayer2DBase->getOutputWidth();
		size_t outputHeight = filterLayer2DBase->getOutputHeight();

		ConvolutionalLayer2D<T>* convolutionalLayer2D = new ConvolutionalLayer2D<T>(
			amountOfOutputMaps, outputWidth, outputHeight, filter);

		return convolutionalLayer2D;
	}
	
	template <typename T>
	static inline FullyConnectedLayer<T>* NeuralLayerGeneration::getCompatibleFullyConnectedLayer<T>(
		NeuralNetworkLayerBase<T>* lastLayer,
		ActivationFunction function,
		size_t newNeuronCount, 
		float dropout)
	{
		NeuralLayerType lastLayerType = lastLayer->getType();
		if (NeuralLayerCompatibility::isAbleToConnectLayerTypes(lastLayerType, NeuralLayerType::FULLYCONNECTED) == false)
		{
			return nullptr;
		}

		size_t neuronCount = lastLayer->getOutputsCount();
		FullyConnectedLayer<T>* newFullyConnectedLayer = 
			new FullyConnectedLayer<T>(function, neuronCount, newNeuronCount, dropout);
		return newFullyConnectedLayer;
	}

	template <typename T>
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
	}
}

#endif