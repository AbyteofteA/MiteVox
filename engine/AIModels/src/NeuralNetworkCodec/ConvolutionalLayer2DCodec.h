#ifndef CONVOLUTIONALLAYER2DCODEC_H
#define CONVOLUTIONALLAYER2DCODEC_H

#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2D.h"
#include "engine/AIModels/src/NeuralNetworkCodec/Filter2DCodec.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace aimods
{
    class ConvolutionalLayer2DCodec
    {
    public:

        template <typename T>
        static ConvolutionalLayer2D<T>* fromJSON(fileio::JSON* ConvolutionalLayer2DJSON);

        template <typename T>
        static fileio::JSON* toJSON(ConvolutionalLayer2D<T>* convolutionalLayer2D);
    };

    template <typename T>
    ConvolutionalLayer2D<T>* ConvolutionalLayer2DCodec::fromJSON(fileio::JSON* layerJSON)
    {
        ActivationFunction function =
            stringToActivationFunction(layerJSON->getFieldStringOrDefault("function", "IDENTITY").c_str());
        size_t amountOfInputMaps = (size_t)layerJSON->getFieldNumberOrDefault("amountOfInputMaps", 0.0f);
        size_t inputWidth = (size_t)layerJSON->getFieldNumberOrDefault("inputWidth", 0.0f);
        size_t inputHeight = (size_t)layerJSON->getFieldNumberOrDefault("inputHeight", 0.0f);

        Filter2D<T>* filter = Filter2DCodec::fromJSON<T>(layerJSON->getFieldObject("filter"));

        ConvolutionalLayer2D<T>* convolutionalLayer2D =
            new ConvolutionalLayer2D<T>(amountOfInputMaps, inputWidth, inputHeight, filter);

        return convolutionalLayer2D;
    }

    template <typename T>
    fileio::JSON* ConvolutionalLayer2DCodec::toJSON(ConvolutionalLayer2D<T>* convolutionalLayer2D)
    {
        fileio::JSON* convolutionalLayer2DJSON = new fileio::JSON();
        convolutionalLayer2DJSON->setField("type", std::string(neuralLayerTypeToString(convolutionalLayer2D->getType())));
        convolutionalLayer2DJSON->setField("function", std::string(activationFunctionToString(convolutionalLayer2D->getFunction())));
        convolutionalLayer2DJSON->setField("amountOfInputMaps", (double)convolutionalLayer2D->getAmountOfInputMaps());
        convolutionalLayer2DJSON->setField("inputWidth", (double)convolutionalLayer2D->getInputWidth());
        convolutionalLayer2DJSON->setField("inputHeight", (double)convolutionalLayer2D->getInputHeight());

        fileio::JSON* filterJSON = Filter2DCodec::toJSON<T>(convolutionalLayer2D->filter);
        convolutionalLayer2DJSON->setField(filterJSON);

        return convolutionalLayer2DJSON;
    }
}

#endif