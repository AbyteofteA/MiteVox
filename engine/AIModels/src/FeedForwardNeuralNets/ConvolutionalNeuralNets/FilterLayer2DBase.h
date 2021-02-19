
#ifndef FILTERLAYER2DBASE_H
#define FILTERLAYER2DBASE_H

#include "AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"

#include <cstdlib>
#include <cstring>

namespace aimods
{
	template <typename T>
	class FilterLayer2DBase
	{
	public:

		Filter2D<T>* filter;
		T* outputs;

		inline FilterLayer2DBase(
			size_t _amountOfInputMaps,
			size_t _inputWidth,
			size_t _inputHeight,
			Filter2D<T>* _filter);
		inline ~FilterLayer2DBase();

		inline void setOutputs(T value);

		// Getters //

		inline size_t getInputsSize();
		inline size_t getAmountOfInputMaps();
		inline size_t getInputWidth();
		inline size_t getInputHeight();

		inline size_t getOutputsSize();
		inline size_t getAmountOfOutputMaps();
		inline size_t getOutputWidth();
		inline size_t getOutputHeight();

	private:

		size_t inputsSize;
		size_t amountOfInputMaps;
		size_t inputWidth;
		size_t inputHeight;

		size_t outputsSize;
		size_t amountOfOutputMaps;
		size_t outputWidth;
		size_t outputHeight;

		void tuneOutputs();
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline FilterLayer2DBase<T>::FilterLayer2DBase(
		size_t _amountOfInputMaps,
		size_t _inputWidth,
		size_t _inputHeight,
		Filter2D<T>* _filter)
	{
		amountOfInputMaps = _amountOfInputMaps;
		inputWidth = _inputWidth;
		inputHeight = _inputHeight;
		inputsSize = amountOfInputMaps * inputWidth * inputHeight;

		filter = _filter;

		tuneOutputs();
	}

	template <typename T>
	inline FilterLayer2DBase<T>::~FilterLayer2DBase()
	{
		delete[] outputs;
	}

	template <typename T>
	inline void FilterLayer2DBase<T>::setOutputs(T value)
	{
		for (size_t i = 0; i < outputsSize; i++)
		{
			outputs[i] = value;
		}
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputsSize()
	{
		return inputsSize;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getAmountOfInputMaps()
	{
		return amountOfInputMaps;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputWidth()
	{
		return inputWidth;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputHeight()
	{
		return inputHeight;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputsSize()
	{
		return outputsSize;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getAmountOfOutputMaps()
	{
		return amountOfOutputMaps;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputWidth()
	{
		return outputWidth;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputHeight()
	{
		return outputHeight;
	}

	template <typename T>
	void FilterLayer2DBase<T>::tuneOutputs()
	{
		if (filter == nullptr)
		{
			return;
		}

		long deltaSize = 0;
		float outputWidthFloat = 0;
		float outputHeightFloat = 0;

		switch (filter->paddingType)
		{
		case PaddingType::NO_PADDING:
			deltaSize = -((long)filter->size - 1) / 2;

			outputWidthFloat =
				(float)((long)inputWidth + deltaSize * 2) / (float)filter->stride;
			outputWidth = (size_t)(ceil(outputWidthFloat));

			outputHeightFloat =
				(float)((long)inputHeight + deltaSize * 2) / (float)filter->stride;
			outputHeight = (size_t)(ceil(outputHeightFloat));
			break;

		case PaddingType::ARBITRARY:
			deltaSize = filter->padding - (filter->size - 1) / 2;

			outputWidthFloat =
				(float)((long)inputWidth + deltaSize * 2) / (float)filter->stride;
			outputWidth = (size_t)(ceil(outputWidthFloat));

			outputHeightFloat =
				(float)((long)inputHeight + deltaSize * 2) / (float)filter->stride;
			outputHeight = (size_t)(ceil(outputHeightFloat));
			break;

		case PaddingType::SAME:
			filter->stride = 1;
			filter->padding = (filter->size - 1) / 2;
			outputWidth = inputWidth;
			outputHeight = inputHeight;
			break;

		case PaddingType::DILATED:
			break;
		}

		amountOfOutputMaps = amountOfInputMaps * filter->amountOfKernels;
		outputsSize = amountOfOutputMaps * outputWidth * outputHeight;

		outputs = new T[outputsSize];
		setOutputs(0);
	}
}

#endif