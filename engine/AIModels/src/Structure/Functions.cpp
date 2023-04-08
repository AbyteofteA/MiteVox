#include "Functions.h"

#include <cstring>

namespace aimods
{
	const char* activationFunctionToString(ActivationFunction activationFunction)
	{
		switch (activationFunction)
		{
		case aimods::ActivationFunction::IDENTITY:
			return "IDENTITY";
		case aimods::ActivationFunction::BINARY:
			return "BINARY";
		case aimods::ActivationFunction::BIPOLAR:
			return "BIPOLAR";
		case aimods::ActivationFunction::RELU:
			return "RELU";
		case aimods::ActivationFunction::LEAKY_RELU:
			return "LEAKY_RELU";
		case aimods::ActivationFunction::SIGMOID:
			return "SIGMOID";
		case aimods::ActivationFunction::TANH:
			return "TANH";
		case aimods::ActivationFunction::AVERAGE:
			return "AVERAGE";
		case aimods::ActivationFunction::MIN:
			return "MIN";
		case aimods::ActivationFunction::MAX:
			return "MAX";
		default:
			break;
		}
		return "";
	}

	ActivationFunction stringToActivationFunction(const char* string)
	{
		if (std::strcmp(string, "IDENTITY") == 0)
		{
			return ActivationFunction::IDENTITY;
		}
		else if (std::strcmp(string, "BINARY") == 0)
		{
			return ActivationFunction::BINARY;
		}
		else if (std::strcmp(string, "BIPOLAR") == 0)
		{
			return ActivationFunction::BIPOLAR;
		}
		else if (std::strcmp(string, "RELU") == 0)
		{
			return ActivationFunction::RELU;
		}
		else if (std::strcmp(string, "LEAKY_RELU") == 0)
		{
			return ActivationFunction::LEAKY_RELU;
		}
		else if (std::strcmp(string, "SIGMOID") == 0)
		{
			return ActivationFunction::SIGMOID;
		}
		else if (std::strcmp(string, "TANH") == 0)
		{
			return ActivationFunction::TANH;
		}
		else if (std::strcmp(string, "AVERAGE") == 0)
		{
			return ActivationFunction::AVERAGE;
		}
		else if (std::strcmp(string, "MIN") == 0)
		{
			return ActivationFunction::MIN;
		}
		else if (std::strcmp(string, "MAX") == 0)
		{
			return ActivationFunction::MAX;
		}
		return ActivationFunction::IDENTITY;
	}
}