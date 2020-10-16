#pragma once


namespace aimods
{
	// Activation functions
#define IDENTITY 0	// Seems like it's useless
#define BINARY_STEP 1
#define BIPOLAR 2
#define ReLU 3
#define SIGMOID 4
#define TanH 5

#define UNDEF_STR -1
#define INT_STR 0
#define CHAR_STR 1

	float activationFunction(float weightedSum, unsigned int function)
	{
		float f = 0;

		switch (function)
		{

		case IDENTITY:
			f = weightedSum;
			break;

		case BINARY_STEP:
			if (weightedSum > 0)
				f = 1;
			else
				f = 0;
			break;

		case BIPOLAR:
			if (weightedSum > 0)
				f = 1;
			else
				f = -1;
			break;

		case ReLU:
			if (weightedSum >= 0)
				f = weightedSum;
			else
				f = weightedSum * 0.05f;
			break;

		case SIGMOID:
			f = 1 / (float)(1 + pow(M_E, (double)-weightedSum));
			break;

		case TanH:
			f = tanh(weightedSum);
			break;
		}

		return f;
	}


	float activationDerivative(float weightedSum, unsigned int function)
	{
		float der = 0;

		switch (function)
		{

		case IDENTITY:
			der = 1;
			break;

		case BINARY_STEP:
			if (weightedSum != 0)
				der = 0;
			else
				der = 0;	// ??? clarify
			break;

		case BIPOLAR:	// ??? clarify
			if (weightedSum != 0)
				der = 0;
			else
				der = 0;
			break;

		case ReLU:
			if (weightedSum >= 0)
				der = 1;
			else
				der = 0.05f;
			break;

		case SIGMOID:
			der = (1 - 1 / (1 + pow((float)M_E, -weightedSum))) / (1 + pow((float)M_E, -weightedSum));
			break;

		case TanH:
			der = 1 - pow(tanh(weightedSum), 2);
			break;
		}

		return der;
	}

}