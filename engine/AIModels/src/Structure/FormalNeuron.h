
#ifndef FORMALNEURON_H
#define FORMALNEURON_H

#include "engine/AIModels/src/Structure/Functions.h"

#include <cmath>

namespace aimods
{
	class FormalNeuron
	{
	public:

		float output = 0;
		float threshold = 0;

		float* weightArray = nullptr;
		FormalNeuron** axonArray = nullptr;
		
		inline FormalNeuron(ActivationFunction _function, unsigned int _amountOfAxons);
		inline ~FormalNeuron();

		inline void setWeights(float value);
		inline void setWeights();

		inline float weighSum();
		inline float computeOutput();
		inline float propagate();

		// Getters //

		inline ActivationFunction getFunction();
		inline size_t getAmountOfAxons();

	private:

		ActivationFunction function = ActivationFunction::RELU;
		size_t amountOfAxons = 0;
	};
}

#endif