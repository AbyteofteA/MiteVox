
#ifndef NEURALNETWORKLAYERBASESERIALIZER_H
#define NEURALNETWORKLAYERBASESERIALIZER_H

#include "NeuralNetworkLayerBase.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace aimods
{
    template <typename T>
    fileio::JSON* NeuralNetworkLayerBaseToJSON(NeuralNetworkLayerBase<T>* neuralNetworkLayerBase)
    {
        fileio::JSON* json = new fileio::JSON();

        // Serialize general neural network Layer's information.

        json->setField("type", (double)neuralNetworkLayerBase->getType());
        json->setField("function", (double)neuralNetworkLayerBase->getFunction());
        json->setField("inputsCount", (double)neuralNetworkLayerBase->getInputsCount());
        json->setField("outputsCount", (double)neuralNetworkLayerBase->getOutputsCount());

        return json;
    }

}

#endif