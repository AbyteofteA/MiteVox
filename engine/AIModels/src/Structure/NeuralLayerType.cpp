#include "NeuralLayerType.h"

#include <cstring>

namespace aimods
{
	const char* neuralLayerTypeToString(NeuralLayerType neuralLayerType)
	{
		switch (neuralLayerType)
		{
		case NeuralLayerType::FULLYCONNECTED:
			return "FULLYCONNECTED";
		case NeuralLayerType::SOFTMAX:
			return "SOFTMAX";
		case NeuralLayerType::CONVOLUTIONAL_2D:
			return "CONVOLUTIONAL_2D";
		case NeuralLayerType::CONVOLUTIONAL_SPATIALLY_SEPARABLE_2D:
			return "CONVOLUTIONAL_SPATIALLY_SEPARABLE_2D";
		case NeuralLayerType::CONVOLUTIONAL_DEPTHWISE_SEPARABLE_2D:
			return "CONVOLUTIONAL_DEPTHWISE_SEPARABLE_2D";
		case NeuralLayerType::CONVOLUTIONAL_DIALATED_2D:
			return "CONVOLUTIONAL_DIALATED_2D";
		case NeuralLayerType::CONVOLUTIONAL_1X1:
			return "CONVOLUTIONAL_1X1";
		case NeuralLayerType::SUBSAMPLING_2D:
			return "SUBSAMPLING_2D";
		case NeuralLayerType::DECONVOLUTIONAL_2D:
			return "DECONVOLUTIONAL_2D";
		case NeuralLayerType::UPSAMPLING_2D:
			return "UPSAMPLING_2D";
		case NeuralLayerType::HOPFIELD:
			return "HOPFIELD";
		default:
			break;
		}
		return "INVALID";
	}

	NeuralLayerType stringToNeuralLayerType(const char* string)
	{
		if (std::strcmp(string, "FULLYCONNECTED") == 0)
		{
			return NeuralLayerType::FULLYCONNECTED;
		}
		else if (std::strcmp(string, "SOFTMAX") == 0)
		{
			return NeuralLayerType::SOFTMAX;
		}
		else if (std::strcmp(string, "CONVOLUTIONAL_2D") == 0)
		{
			return NeuralLayerType::CONVOLUTIONAL_2D;
		}
		else if (std::strcmp(string, "CONVOLUTIONAL_SPATIALLY_SEPARABLE_2D") == 0)
		{
			return NeuralLayerType::CONVOLUTIONAL_SPATIALLY_SEPARABLE_2D;
		}
		else if (std::strcmp(string, "CONVOLUTIONAL_DEPTHWISE_SEPARABLE_2D") == 0)
		{
			return NeuralLayerType::CONVOLUTIONAL_DEPTHWISE_SEPARABLE_2D;
		}
		else if (std::strcmp(string, "CONVOLUTIONAL_DIALATED_2D") == 0)
		{
			return NeuralLayerType::CONVOLUTIONAL_DIALATED_2D;
		}
		else if (std::strcmp(string, "CONVOLUTIONAL_1X1") == 0)
		{
			return NeuralLayerType::CONVOLUTIONAL_1X1;
		}
		else if (std::strcmp(string, "SUBSAMPLING_2D") == 0)
		{
			return NeuralLayerType::SUBSAMPLING_2D;
		}
		else if (std::strcmp(string, "DECONVOLUTIONAL_2D") == 0)
		{
			return NeuralLayerType::DECONVOLUTIONAL_2D;
		}
		else if (std::strcmp(string, "UPSAMPLING_2D") == 0)
		{
			return NeuralLayerType::UPSAMPLING_2D;
		}
		else if (std::strcmp(string, "HOPFIELD") == 0)
		{
			return NeuralLayerType::HOPFIELD;
		}
		return NeuralLayerType::INVALID;
	}
}