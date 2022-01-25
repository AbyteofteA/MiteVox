
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

		/// <summary>
		/// Propagates signals through the layer, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size FilterLayer2DBase::_inputsDataSize</param>
		inline void propagate(T* inputs);
		inline void propagateSavingWeightedSums(T* inputs);
		inline void adjustWeights(T* inputs, float learningRate) {}

	private:

		void tuneOutputs();
		inline void computeWeightedSums(T* inputs, T* resultsArray);
		inline void computeOutputs(T* weightedSumsArray);
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
	inline void ConvolutionalLayer2D<T>::propagate(T* inputs)
	{
		computeWeightedSums(inputs, this->_outputs);
		computeWeightedSums(this->_temporaryCalculations, this->_outputs);
	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::propagateSavingWeightedSums(T* inputs)
	{
		computeWeightedSums(inputs, this->_temporaryCalculations);
		computeWeightedSums(this->_temporaryCalculations, this->_outputs);
	}

	//template <typename T>
	//inline void ConvolutionalLayer2D<T>::adjustWeights(T* inputs, float learningRate);

	template <typename T>
	void ConvolutionalLayer2D<T>::tuneOutputs()
	{
		this->_outputMapElementCount = this->_outputWidth * this->_outputHeight;
		this->_outputsCount = this->_outputMapElementCount * this->_amountOfOutputMaps;
		this->_outputs = new T[this->_outputsCount];
		this->_outputsDataSize = this->_outputsCount * sizeof(T);
	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::computeWeightedSums(T* inputs, T* resultsArray)
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

		long filterDimension = this->_filter->getDimension();
		long filterHalfDimension = this->_filter->computeHalfDimension();
		long filterStride = this->_filter->getStride();
		T* filterData = this->_filter->getFilterData();

		this->setOutputs(0);

		
	}

	template <typename T>
	inline void ConvolutionalLayer2D<T>::computeOutputs(T* weightedSumsArray) 
	{

	}
}

#endif