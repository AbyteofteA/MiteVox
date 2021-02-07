
#ifndef CONVLAYER2D_H
#define CONVLAYER2D_H

#include "AIModels/src/FFNN/CNN/Filter2D.h"
#include "AIModels/src/FFNN/CNN/FilterLayer2D.h"

namespace aimods
{
	template <typename T>
	class ConvoLayer2D : public FilterLayer2D<T>
	{
	public:

		ConvoLayer2D(
			size_t _amountOfInputs, 
			size_t _inputWidth, 
			size_t _inputHeight,
			Filter2D<T>* _filter) : FilterLayer2D<T>(
				_amountOfInputs,
				_inputWidth,
				_inputHeight,
				_filter) {}


	};
}

#endif