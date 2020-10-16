#pragma once

namespace aimods
{

	class Convolution_layer
	{
	public:

		unsigned int amountOfInputs = 0;
		aimods::HyperData<float>** inputs = NULL;

		unsigned int amountOfOutputs = 0;
		aimods::HyperData<float>** outputs = NULL;

		unsigned int amountOfilters = 0;
		Filter_2D** filters = NULL;

		Convolution_layer()
		{
			amountOfilters = 0;
			filters = NULL;
		}

		void addFilter(unsigned int _amountOfChannels, unsigned int _size, unsigned int _stride, unsigned char _padding)
		{
			amountOfilters++;

			filters = (Filter_2D**)realloc(filters, sizeof(Filter_2D*) * amountOfilters);
			filters[amountOfilters - 1] = new Filter_2D(_amountOfChannels, _size, _stride, _padding);
		}

		void addFilter(Filter_2D* filter)
		{
			amountOfilters++;

			filters = (Filter_2D**)realloc(filters, sizeof(Filter_2D*) * amountOfilters);
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

		~Convolution_layer()
		{
			delete[] filters;
		}
	};
}