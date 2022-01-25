
#ifndef FULLYCONNECTEDLAYERSERIALIZER_H
#define FULLYCONNECTEDLAYERSERIALIZER_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "FullyConnectedLayer.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBaseSerializer.h"

#include <string>

namespace aimods
{
    template <typename T>
    fileio::JSON* FullyConnectedLayerToJSON(FullyConnectedLayer<T>* fullyConnectedLayer)
    {
        fileio::JSON* json = new fileio::JSON();

        fileio::JSON* neuralNetworkLayerBase =
            NeuralNetworkLayerBaseToJSON<T>((NeuralNetworkLayerBase<T>*)fullyConnectedLayer);
        neuralNetworkLayerBase->setName("NeuralNetworkLayerBase");
        json->setField(neuralNetworkLayerBase);

        // Serialize general FullyConnectedLayer's information.

        fileio::JSON* fullyConnectedLayerJSON = json->setFieldType("FullyConnectedLayer", fileio::JSONtype::OBJECT);
        fullyConnectedLayerJSON->setField("dropout", (double)fullyConnectedLayer->getDropout());

        // Serialize weights.

        size_t weightsCount = fullyConnectedLayer->getWeightsCount();
        T* weights = fullyConnectedLayer->getWeights();

        fullyConnectedLayerJSON->setField("weightsCount", (double)weightsCount);
        fileio::JSON* weightsArrayJSON = fullyConnectedLayerJSON->setFieldType("weights", fileio::JSONtype::ARRAY);
        for (size_t weightIndex = 0; weightIndex < weightsCount; weightIndex++)
        {
            weightsArrayJSON->addArrayItem((double)weights[weightIndex]);
        }

        // Serialize thresholds.

        size_t outputsCount = fullyConnectedLayer->getOutputsCount();
        T* thresholds = fullyConnectedLayer->getThresholds();

        fileio::JSON* thresholdsArrayJSON = fullyConnectedLayerJSON->setFieldType("thresholds", fileio::JSONtype::ARRAY);
        for (size_t thresholdIndex = 0; thresholdIndex < outputsCount; thresholdIndex++)
        {
            thresholdsArrayJSON->addArrayItem((double)thresholds[thresholdIndex]);
        }

        return json;
    }
}

#endif