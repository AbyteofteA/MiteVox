
#ifndef FILTERLAYER2DBASE_H
#define FILTERLAYER2DBASE_H

#include "AIModels/src/FFNN/CNN/Filter2D.h"

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
			size_t _amountOfInputs,
			size_t _inputWidth,
			size_t _inputHeight,
			Filter2D<T>* _filter);
		inline ~FilterLayer2DBase();

		// Input getters //

		inline size_t getAmountOfInputs();
		inline size_t getInputWidth();
		inline size_t getInputHeight();

		// Output getters //

		inline size_t getAmountOfOutputs();
		inline size_t getOutputWidth();
		inline size_t getOutputHeight();

	private:

		size_t amountOfInputs;
		size_t inputWidth;
		size_t inputHeight;

		size_t outputsSizeInBytes;
		size_t amountOfOutputs;
		size_t outputWidth;
		size_t outputHeight;

		void tuneOutputs();
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline FilterLayer2DBase<T>::FilterLayer2DBase(
		size_t _amountOfInputs,
		size_t _inputWidth,
		size_t _inputHeight,
		Filter2D<T>* _filter)
	{
		amountOfInputs = _amountOfInputs;
		inputWidth = _inputWidth;
		inputHeight = _inputHeight;

		filter = _filter;

		tuneOutputs();
	}

	template <typename T>
	inline FilterLayer2DBase<T>::~FilterLayer2DBase()
	{
		free(outputs);
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getAmountOfInputs()
	{
		return amountOfInputs;
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
	inline size_t FilterLayer2DBase<T>::getAmountOfOutputs()
	{
		return amountOfOutputs;
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

		amountOfOutputs = amountOfInputs * filter->amountOfKernels;

		outputsSizeInBytes =
			outputWidth * outputHeight * amountOfOutputs * sizeof(T);
		outputs = (T*)realloc(outputs, outputsSizeInBytes);
		memset(outputs, 0, outputsSizeInBytes);
	}
}

#endif