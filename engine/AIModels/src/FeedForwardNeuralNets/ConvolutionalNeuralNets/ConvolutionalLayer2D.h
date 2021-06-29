
#ifndef CONVOLUTIONALLAYER2D_H
#define CONVOLUTIONALLAYER2D_H

#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/FilterLayer2DBase.h"

namespace aimods
{
	template <typename T>
	class ConvolutionalLayer2D : public FilterLayer2DBase<T>
	{
	public:

		ConvolutionalLayer2D(
			size_t amountOfInputMaps,
			size_t inputWidth,
			size_t inputHeight,
			Filter2D<T>* filter);

		inline void weighSum(T* inputs);
		inline void computeOutput();

		/// <summary>
		/// Propagates signals through the layer, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size FilterLayer2DBase::_inputsDataSize</param>
		inline void propagate(T* inputs);
		inline void propagateSavingWeightedSums(T* inputs) {}
		inline void adjustWeights(T* inputs, float learningRate) {}

	private:

		inline void computeWeightedSums(T* inputs, T* resultsArray) {}
		inline void computeOutputs(T* weightedSumsArray) {}
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	ConvolutionalLayer2D<T>::ConvolutionalLayer2D(
		size_t amountOfInputMaps,
		size_t inputWidth,
		size_t inputHeight,
		Filter2D<T>* filter) : FilterLayer2DBase<T>(
			NeuralLayerType::CONVOLUTIONAL_2D,
			amountOfInputMaps,
			inputWidth,
			inputHeight,
			filter)
	{

	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::weighSum(T* inputs) {}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::computeOutput() {}
	
	template <typename T>
	inline void ConvolutionalLayer2D<T>::propagate(T* inputs)
	{
		// Check if the inputs are passed.
		if (inputs == nullptr)
		{
			return;
		}

		size_t amountOfInputMaps = this->getAmountOfInputMaps();
		size_t inputHeight = this->getInputHeight();
		size_t inputWidth = this->getInputWidth();

		size_t amountOfOutputMaps = this->getAmountOfOutputMaps();
		size_t outputHeight = this->getOutputHeight();
		size_t outputWidth = this->getOutputWidth();

		long filterHalfDimension = this->_filter->computeHalfDimension();
		long filterStride = this->_filter->getStride();
	}
}

#endif