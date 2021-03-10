
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <conio.h>

namespace aimods
{
	enum struct activation { LINEAR = 0, BINARY, BIPOLAR, RELU, SIGMOID, TANH };

	inline float activationFunction(float weightedSum, activation function)
	{
		float f = 0;

		switch (function)
		{

		case activation::LINEAR:
			f = weightedSum;
			break;

		case activation::BINARY:
			if (weightedSum > 0)
				f = 1;
			else
				f = 0;
			break;

		case activation::BIPOLAR:
			if (weightedSum > 0)
				f = 1;
			else
				f = -1;
			break;

		case activation::RELU:
			if (weightedSum >= 0)
				f = weightedSum;
			else
				f = weightedSum * 0.05f;
			break;

		case activation::SIGMOID:
			f = 1.0f / (1.0f + (float)pow((float)M_E, -weightedSum));
			break;

		case activation::TANH:
			f = (float)tanh(weightedSum);
			break;
		}

		return f;
	}


	inline float activationDerivative(float weightedSum, activation function)
	{
		float der = 0;

		switch (function)
		{

		case activation::LINEAR:
			der = 1;
			break;

		case activation::BINARY:
			if (weightedSum != 0)
				der = 0;
			else
				der = 0;	// ??? clarify
			break;

		case activation::BIPOLAR:	// ??? clarify
			if (weightedSum != 0)
				der = 0;
			else
				der = 0;
			break;

		case activation::RELU:
			if (weightedSum >= 0)
				der = 1;
			else
				der = 0.05f;
			break;

		case activation::SIGMOID:
			der = (1.0f - 1.0f / (1.0f + (float)pow((float)M_E, -weightedSum))) / 
				(1.0f + (float)pow((float)M_E, -weightedSum));
			break;

		case activation::TANH:
			der = 1.0f - (float)pow(tanh(weightedSum), 2.0f);
			break;
		}

		return der;
	}

}

#endif