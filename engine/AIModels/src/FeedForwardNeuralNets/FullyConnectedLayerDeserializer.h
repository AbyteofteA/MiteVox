
#ifndef FULLYCONNECTEDLAYERDESERIALIZER_H
#define FULLYCONNECTEDLAYERDESERIALIZER_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "FullyConnectedLayer.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBaseSerializer.h"

#include <string>

namespace aimods
{
    template <typename T>
    FullyConnectedLayer<T>* FullyConnectedLayerFromJSON(fileio::JSON* layerJSON)
    {
        // Deserialize NeuralNetworkLayerBase's information.

        fileio::JSON* neuralNetworkLayerBaseJSON = layerJSON->getFieldObject("NeuralNetworkLayerBase");
        ActivationFunction function = 
            (ActivationFunction)neuralNetworkLayerBaseJSON->getFieldNumber("function");
        size_t inputsCount = (size_t)neuralNetworkLayerBaseJSON->getFieldNumber("inputsCount");
        size_t outputsCount = (size_t)neuralNetworkLayerBaseJSON->getFieldNumber("outputsCount");

        // Deserialize general FullyConnectedLayer's information.

        fileio::JSON* fullyConnectedLayerJSON = layerJSON->getFieldObject("FullyConnectedLayer");
        float dropout = (float)fullyConnectedLayerJSON->getFieldNumber("dropout");

        // Create and initialize FullyConnectedLayer.

        FullyConnectedLayer<T>* fullyConnectedLayer = 
            new FullyConnectedLayer<T>(function, inputsCount, outputsCount, dropout);

        // Deserialize weights.

        size_t weightsCount = fullyConnectedLayer->getWeightsCount();
        T* weights = fullyConnectedLayer->getWeights();
        fileio::JSON* weightsArrayJSON = fullyConnectedLayerJSON->getFieldArray("weights");

        for (size_t weightIndex = 0; weightIndex < weightsCount; weightIndex++)
        {
            weights[weightIndex] = (T)weightsArrayJSON->getArrayItem(weightIndex)->getNumber();
        }

        // Deserialize thresholds.

        T* thresholds = fullyConnectedLayer->getThresholds();
        fileio::JSON* thresholdsArrayJSON = fullyConnectedLayerJSON->getFieldArray("thresholds");

        for (size_t thresholdIndex = 0; thresholdIndex < outputsCount; thresholdIndex++)
        {
            thresholds[thresholdIndex] = (T)thresholdsArrayJSON->getArrayItem(thresholdIndex)->getNumber();
        }

        return fullyConnectedLayer;
    }
}

#endif