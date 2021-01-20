
#ifndef CONVOLUTIONALLAYER_H
#define CONVOLUTIONALLAYER_H

#include "AIModels/src/Structure/HyperData.h"
#include "AIModels/src/FFNN/CNN/Filter2D.h"

namespace aimods
{
	template <typename T>
	class ConvolutionalLayer
	{

	public:

		unsigned int amountOfInputs = 0;
		aimods::HyperData<float>** inputs = nullptr;

		unsigned int amountOfOutputs = 0;
		aimods::HyperData<float>** outputs = nullptr;

		unsigned int amountOfilters = 0;
		Filter2D** filters = nullptr;

		ConvolutionalLayer()
		{
			amountOfilters = 0;
			filters = nullptr;
		}

		void addFilter(unsigned int _amountOfChannels, unsigned int _size, unsigned int _stride, unsigned char _padding)
		{
			amountOfilters++;

			filters = (Filter2D**)realloc(filters, sizeof(Filter2D*) * amountOfilters);
			filters[amountOfilters - 1] = new Filter2D(_amountOfChannels, _size, _stride, _padding);
		}

		void addFilter(Filter2D* filter)
		{
			amountOfilters++;

			filters = (Filter2D**)realloc(filters, sizeof(Filter2D*) * amountOfilters);
			filters[amountOfilters - 1] = filter;
		}

		void addInput(aimods::HyperData<float>* _input)
		{
			amountOfInputs++;

			inputs = (aimods::HyperData<float>**)realloc(inputs, sizeof(aimods::HyperData<float>*) * amountOfInputs);
			inputs[amountOfInputs - 1] = _input;

			/*input = _input;
			for (unsigned int f = 1; f < amountOfilters; f++)
			{
				filters[f]->attachInput(input);
			}*/
		}

		void tuneOutputs()
		{

		}

		~ConvolutionalLayer()
		{
			delete[] filters;
		}
	};
}

#endif