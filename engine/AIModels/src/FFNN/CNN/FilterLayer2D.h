
#ifndef FILTERLAYER2D_H
#define FILTERLAYER2D_H

#include "AIModels/src/FFNN/CNN/Filter2D.h"

namespace aimods
{
	template <typename T>
	class FilterLayer2D
	{
	public:

		size_t amountOfInputs;
		size_t inputWidth;
		size_t inputHeight;

		Filter2D<T>* filter;

		T* outputs;
		size_t outputsSizeInBytes;
		size_t amountOfOutputs;
		size_t outputWidth;
		size_t outputHeight;

		FilterLayer2D(
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
		~FilterLayer2D()
		{
			free(outputs);
		}

		void tuneOutputs()
		{
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
	};
}

#endif