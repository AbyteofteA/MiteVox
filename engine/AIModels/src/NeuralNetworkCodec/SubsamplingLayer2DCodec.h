#ifndef SUBSAMPLINGLAYER2DCODEC_H
#define SUBSAMPLINGLAYER2DCODEC_H

#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/FilterLayer2DBase.h"
#include "engine/AIModels/src/NeuralNetworkCodec/Filter2DCodec.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace aimods
{
    class SubsamplingLayer2DCodec
    {
    public:

        template <typename T>
        static SubsamplingLayer2D<T>* fromJSON(fileio::JSON* layerJSON);

        template <typename T>
        static fileio::JSON* toJSON(SubsamplingLayer2D<T>* subsamplingLayer2D);
    };

    template <typename T>
    SubsamplingLayer2D<T>* SubsamplingLayer2DCodec::fromJSON(fileio::JSON* layerJSON)
    {
        ActivationFunction function =
            stringToActivationFunction(layerJSON->getFieldStringOrDefault("function", "IDENTITY").c_str());
        size_t amountOfInputMaps = (size_t)layerJSON->getFieldNumberOrDefault("amountOfInputMaps", 0.0f);
        size_t inputWidth = (size_t)layerJSON->getFieldNumberOrDefault("inputWidth", 0.0f);
        size_t inputHeight = (size_t)layerJSON->getFieldNumberOrDefault("inputHeight", 0.0f);

        Filter2D<T>* filter = Filter2DCodec::fromJSON<T>(layerJSON->getFieldObject("filter"));

        T weight = (T)layerJSON->getFieldNumberOrDefault("weight", 0.0f);
        T threshold = (T)layerJSON->getFieldNumberOrDefault("threshold", 0.0f);

        SubsamplingLayer2D<T>* subsamplingLayer2D =
            new SubsamplingLayer2D<T>(amountOfInputMaps, inputWidth, inputHeight, filter);
        subsamplingLayer2D->function = function;
        subsamplingLayer2D->weight = weight;
        subsamplingLayer2D->threshold = threshold;

        return subsamplingLayer2D;
    }

    template <typename T>
    fileio::JSON* SubsamplingLayer2DCodec::toJSON(SubsamplingLayer2D<T>* subsamplingLayer2D)
    {
        fileio::JSON* subsamplingLayer2DJSON = new fileio::JSON();
        subsamplingLayer2DJSON->setField("type", std::string(neuralLayerTypeToString(subsamplingLayer2D->getType())));
        subsamplingLayer2DJSON->setField("function", std::string(activationFunctionToString(subsamplingLayer2D->getFunction())));
        subsamplingLayer2DJSON->setField("amountOfInputMaps", (double)subsamplingLayer2D->getAmountOfInputMaps());
        subsamplingLayer2DJSON->setField("inputWidth", (double)subsamplingLayer2D->getInputWidth());
        subsamplingLayer2DJSON->setField("inputHeight", (double)subsamplingLayer2D->getInputHeight());

        fileio::JSON* filterJSON = Filter2DCodec::toJSON<T>(subsamplingLayer2D->filter);
        subsamplingLayer2DJSON->setField(filterJSON);

        subsamplingLayer2DJSON->setField("weight", (double)subsamplingLayer2D->getWeight());
        subsamplingLayer2DJSON->setField("threshold", (double)subsamplingLayer2D->getThreshold());
        return subsamplingLayer2DJSON;
    }
}

#endif