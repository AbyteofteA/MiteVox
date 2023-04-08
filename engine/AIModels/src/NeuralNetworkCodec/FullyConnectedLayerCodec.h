#ifndef FULLYCONNECTEDLAYERCODEC_H
#define FULLYCONNECTEDLAYERCODEC_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayer.h"

#include <string>

namespace aimods
{
    class FullyConnectedLayerCodec
    {
    public:

        template <typename T>
        static FullyConnectedLayer<T>* fromJSON(fileio::JSON* layerJSON);

        template <typename T>
        static fileio::JSON* toJSON(FullyConnectedLayer<T>* fullyConnectedLayer);
    };

    template <typename T>
    FullyConnectedLayer<T>* FullyConnectedLayerCodec::fromJSON(fileio::JSON* layerJSON)
    {
        ActivationFunction function =
            stringToActivationFunction(layerJSON->getFieldStringOrDefault("function", "IDENTITY").c_str());
        size_t inputsCount = (size_t)layerJSON->getFieldNumberOrDefault("inputsCount", 0.0);
        size_t outputsCount = (size_t)layerJSON->getFieldNumberOrDefault("outputsCount", 0.0);
        float dropout = (float)layerJSON->getFieldNumberOrDefault("dropout", 0.0);

        FullyConnectedLayer<T>* fullyConnectedLayer =
            new FullyConnectedLayer<T>(function, inputsCount, outputsCount, dropout);

        fileio::JSON* weightsArrayJSON = layerJSON->getFieldArray("weights");
        weightsArrayJSON->toNumberArray(&fullyConnectedLayer->weights);

        fileio::JSON* thresholdsArrayJSON = layerJSON->getFieldArray("thresholds");
        thresholdsArrayJSON->toNumberArray(&fullyConnectedLayer->thresholds);

        return fullyConnectedLayer;
    }

    template <typename T>
    fileio::JSON* FullyConnectedLayerCodec::toJSON(FullyConnectedLayer<T>* fullyConnectedLayer)
    {
        fileio::JSON* fullyConnectedLayerJSON = new fileio::JSON();
        fullyConnectedLayerJSON->setField("type", std::string(neuralLayerTypeToString(fullyConnectedLayer->getType())));
        fullyConnectedLayerJSON->setField("function", std::string(activationFunctionToString(fullyConnectedLayer->getFunction())));
        fullyConnectedLayerJSON->setField("inputsCount", (double)fullyConnectedLayer->getInputsCount());
        fullyConnectedLayerJSON->setField("outputsCount", (double)fullyConnectedLayer->getOutputsCount());
        fullyConnectedLayerJSON->setField("dropout", (double)fullyConnectedLayer->getDropout());

        fileio::JSON* weightsArrayJSON = fullyConnectedLayerJSON->setFieldType("weights", fileio::JSONtype::ARRAY);
        weightsArrayJSON->fromNumberArray(&fullyConnectedLayer->weights);

        fileio::JSON* thresholdsArrayJSON = fullyConnectedLayerJSON->setFieldType("thresholds", fileio::JSONtype::ARRAY);
        thresholdsArrayJSON->fromNumberArray(&fullyConnectedLayer->thresholds);

        return fullyConnectedLayerJSON;
    }
}

#endif