
#ifndef SUBSAMPLINGLAYER2D_H
#define SUBSAMPLINGLAYER2D_H

#include "AIModels/src/FFNN/CNN/Filter2D.h"
#include "AIModels/src/FFNN/CNN/FilterLayer2DBase.h"

namespace aimods
{
	template <typename T>
	class SubsamplingLayer2D : public FilterLayer2DBase<T>
	{
	public:

		SubsamplingLayer2D(
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