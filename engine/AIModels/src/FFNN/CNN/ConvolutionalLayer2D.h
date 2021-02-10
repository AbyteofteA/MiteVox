
#ifndef CONVOLUTIONALLAYER2D_H
#define CONVOLUTIONALLAYER2D_H

#include "AIModels/src/FFNN/CNN/Filter2D.h"
#include "AIModels/src/FFNN/CNN/FilterLayer2DBase.h"

namespace aimods
{
	template <typename T>
	class ConvolutionalLayer2D : public FilterLayer2DBase<T>
	{
	public:

		ConvolutionalLayer2D(
			size_t _amountOfInputs, 
			size_t _inputWidth, 
			size_t _inputHeight,
			Filter2D<T>* _filter) : FilterLayer2DBase<T>(
				_amountOfInputs,
				_inputWidth,
				_inputHeight,
				_filter) {}


	};
}

#endif