#include "FormalNeuron.h"

#include "engine/AIModels/src/Structure/Functions.h"

#include <cmath>

namespace aimods
{

	inline FormalNeuron::FormalNeuron(ActivationFunction _function, unsigned int _amountOfAxons)
	{
		this->function = _function;
		this->amountOfAxons = _amountOfAxons;

		this->axonArray = new FormalNeuron * [this->amountOfAxons];
		this->weightArray = new float[this->amountOfAxons];

		setWeights();
	}

	inline FormalNeuron::~FormalNeuron()
	{
		delete[] this->axonArray;
		delete[] this->weightArray;
	}

	inline void FormalNeuron::setWeights(float value)
	{
		for (unsigned int a = 0; a < this->amountOfAxons; a++)
		{
			this->weightArray[a] = value;
		}
	}

	inline void FormalNeuron::setWeights()
	{
		for (unsigned int a = 0; a < this->amountOfAxons; a++)
		{
			this->weightArray[a] = (rand() % 20 - 10) / (float)1000;
		}
	}

	inline float FormalNeuron::weighSum()
	{
		float wSum = 0;
		for (unsigned int k = 0; k < this->amountOfAxons; k++)
			wSum += this->axonArray[k]->output * this->weightArray[k];
		wSum -= this->threshold;

		return wSum;
	}

	inline float FormalNeuron::computeOutput()
	{
		return activationFunction(weighSum(), this->function);
	}

	inline float FormalNeuron::propagate()
	{
		this->output = activationFunction(weighSum(), this->function);
		return this->output;
	}

	inline ActivationFunction FormalNeuron::getFunction()
	{
		return function;
	}

	inline size_t FormalNeuron::getAmountOfAxons()
	{
		return amountOfAxons;
	}
}