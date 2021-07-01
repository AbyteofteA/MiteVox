
#ifndef SUBSAMPLINGLAYER2DDESERIALIZER_H
#define SUBSAMPLINGLAYER2DDESERIALIZER_H

#include "engine/FileIO/src/Formats/JSON.h"
#include "SubsamplingLayer2D.h"
#include "engine/AIModels/src/Structure/Functions.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBaseSerializer.h"
#include "Filter2D.h"
#include "Filter2DDeserializer.h"

#include <string>

namespace aimods
{
    template <typename T>
    SubsamplingLayer2D<T>* SubsamplingLayer2DFromJSON(fileio::JSON* layerJSON)
    {
        fileio::JSON* filterLayer2DBase = layerJSON->getFieldObject("FilterLayer2DBase");

        // Deserialize general SubsamplingLayer2D's information.

        size_t amountOfInputMaps = (size_t)filterLayer2DBase->getFieldNumber("amountOfInputMaps");
        size_t inputWidth = (size_t)filterLayer2DBase->getFieldNumber("inputWidth");
        size_t inputHeight = (size_t)filterLayer2DBase->getFieldNumber("inputHeight");

        // Create and initialize Filter2D.

        Filter2D<T>* filter = Filter2DFromJSON<T>(filterLayer2DBase->getFieldObject("filter"));

        fileio::JSON* subsamplingLayer2DJSON = layerJSON->getFieldObject("SubsamplingLayer2D");
        T weight = (T)subsamplingLayer2DJSON->getFieldNumber("weight");
        T threshold = (T)subsamplingLayer2DJSON->getFieldNumber("threshold");
        ActivationFunction function = (ActivationFunction)subsamplingLayer2DJSON->getFieldNumber("function");

        SubsamplingLayer2D<T>* subsamplingLayer2D =
            new SubsamplingLayer2D<T>(amountOfInputMaps, inputWidth, inputHeight, function, filter);
        subsamplingLayer2D->_weight = weight;
        subsamplingLayer2D->_threshold = threshold;

        return subsamplingLayer2D;
    }
}

#endif