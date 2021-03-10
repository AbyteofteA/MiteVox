
#ifndef SUBSAMPLINGLAYER2D_H
#define SUBSAMPLINGLAYER2D_H

#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/FilterLayer2DBase.h"

namespace aimods
{
	template <typename T>
	class SubsamplingLayer2D : public FilterLayer2DBase<T>
	{
	public:

		SubsamplingLayer2D(
			size_t _amountOfInputMaps,
			size_t _inputWidth,
			size_t _inputHeight,
			Filter2D<T>* _filter) : FilterLayer2DBase<T>(
				_amountOfInputMaps,
				_inputWidth,
				_inputHeight,
				_filter) {}

		inline void resetOutputs();

		/// <summary>
		/// Propagates signals through the layer, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size "filter->inputsSize"</param>
		inline void propagate(T* inputs);
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline void SubsamplingLayer2D<T>::resetOutputs()
	{
		this->setOutputs(0);
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::propagate(T* inputs)
	{
		size_t amountOfOutputMaps = this->getAmountOfOutputMaps();
		for (size_t i = 0; i < amountOfOutputMaps; i++)
		{

		}
	}
}

#endif