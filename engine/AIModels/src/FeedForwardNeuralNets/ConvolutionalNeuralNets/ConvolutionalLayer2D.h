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

		inline void setWeightsRandom();
		inline void setWeights(T value);

		/// <summary>
		/// Propagates signals through the layer, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size FilterLayer2DBase::inputsDataSize</param>
		inline void propagate(safety::SafeArray<T>* inputs);
		inline void propagateSavingWeightedSums(safety::SafeArray<T>* inputs);
		inline void adjustWeights(safety::SafeArray<T>* inputs, float learningRate);

	private:

		void tuneOutputs();
		inline void computeWeightedSums(safety::SafeArray<T>* inputs, safety::SafeArray<T>* resultsArray);
		inline void computeOutputs(safety::SafeArray<T>* weightedSumsArray);
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
			1,
			filter)
	{
		this->tuneOutputs();
	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::setWeightsRandom()
	{
		this->filter->setFilterRandom();
	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::setWeights(T value)
	{
		this->filter->setFilter(value);
	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::propagate(safety::SafeArray<T>* inputs)
	{
		computeWeightedSums(inputs, &this->outputs);
		computeOutputs(&this->outputs);
	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::propagateSavingWeightedSums(safety::SafeArray<T>* inputs)
	{
		computeWeightedSums(inputs, &this->temporaryCalculations);
		computeOutputs(&this->temporaryCalculations);
	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::adjustWeights(safety::SafeArray<T>* inputs, float learningRate)
	{
		// TODO:
	}

	template <typename T>
	void ConvolutionalLayer2D<T>::tuneOutputs()
	{
		this->outputMapElementCount = this->outputWidth * this->outputHeight;
		size_t outputsCount = this->outputMapElementCount * this->amountOfOutputMaps;
		this->outputs.resize(outputsCount);
		this->outputsDataSize = outputsCount * sizeof(T);
	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::computeWeightedSums(safety::SafeArray<T>* inputs, safety::SafeArray<T>* resultsArray)
	{
		if (inputs->getElementsCount() == 0)
		{
			return;
		}

		size_t amountOfInputMaps = this->getAmountOfInputMaps();
		size_t inputHeight = this->getInputHeight();
		size_t inputWidth = this->getInputWidth();

		size_t amountOfOutputMaps = this->getAmountOfOutputMaps();
		size_t outputHeight = this->getOutputHeight();
		size_t outputWidth = this->getOutputWidth();

		long filterDimension = this->filter->getDimension();
		long filterHalfDimension = this->filter->computeHalfDimension();
		long filterStride = this->filter->getStride();
		T* filterData = this->filter->getFilterData();

		this->outputs.setAllElements(0);

		// TODO:
	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::computeOutputs(safety::SafeArray<T>* weightedSumsArray)
	{
		// TODO:
	}
}

#endif