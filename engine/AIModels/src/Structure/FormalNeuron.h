
#ifndef FORMALNEURON_H
#define FORMALNEURON_H

#include "AIModels/src/Structure/Functions.h"

#include <iostream>

namespace aimods
{
	class FormalNeuron
	{
	public:

		activation function = activation::RELU;

		float output = 0;
		float threshold = 0;

		float* weightArray = nullptr;
		FormalNeuron** axonArray = nullptr;
		unsigned int amountOfAxons = 0;

		inline FormalNeuron() {}
		inline FormalNeuron(activation _function, unsigned int _amountOfAxons)
		{
			this->function = _function;
			formAxons(_amountOfAxons);
		}
		inline ~FormalNeuron()
		{
			free(this->axonArray);
			free(this->weightArray);
		}

		inline void formAxons(unsigned int _amountOfAxons)
		{
			this->amountOfAxons = _amountOfAxons;

			this->axonArray = (FormalNeuron**)realloc(this->axonArray, sizeof(FormalNeuron*) * _amountOfAxons);
			this->weightArray = (float*)realloc(this->weightArray, sizeof(float) * _amountOfAxons);

			setWeights();
		}

		inline void setWeights(float value)
		{
			for (unsigned int a = 0; a < this->amountOfAxons; a++)
			{
				this->weightArray[a] = value;
			}
		}
		inline void setWeights()
		{
			for (unsigned int a = 0; a < this->amountOfAxons; a++)
			{
				this->weightArray[a] = (rand() % 20 - 10) / (float)1000;
			}
		}

		inline float weighSum()
		{
			float wSum = 0;
			for (unsigned int k = 0; k < this->amountOfAxons; k++)
				wSum += this->axonArray[k]->output * this->weightArray[k];
			wSum -= this->threshold;

			return wSum;
		}
		inline float computeOutput()
		{
			return activationFunction(weighSum(), this->function);
		}
		inline float propagate()
		{
			this->output = activationFunction(weighSum(), this->function);
			return this->output;
		}
	};
}

#endif