
#ifndef CONVOLUTIONALLAYER2DDESERIALIZER_H
#define CONVOLUTIONALLAYER2DDESERIALIZER_H

#include "engine/FileIO/src/Formats/JSON.h"
#include "ConvolutionalLayer2D.h"
#include "engine/AIModels/src/Structure/Functions.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBaseSerializer.h"
#include "Filter2D.h"
#include "Filter2DDeserializer.h"

#include <string>

namespace aimods
{
    template <typename T>
    ConvolutionalLayer2D<T>* ConvolutionalLayer2DFromJSON(fileio::JSON* convolutionalLayer2DJSON)
    {
        fileio::JSON* filterLayer2DBase = convolutionalLayer2DJSON->getFieldObject("FilterLayer2DBase");

        // Deserialize general ConvolutionalLayer2D's information.

        size_t amountOfInputMaps = (size_t)filterLayer2DBase->getFieldNumber("amountOfInputMaps");
        size_t inputWidth = (size_t)filterLayer2DBase->getFieldNumber("inputWidth");
        size_t inputHeight = (size_t)filterLayer2DBase->getFieldNumber("inputHeight");

        // Create and initialize Filter2D.

        Filter2D<T>* filter = Filter2DFromJSON<T>(filterLayer2DBase->getFieldObject("filter"));

        ConvolutionalLayer2D<T>* convolutionalLayer2D =
            new ConvolutionalLayer2D<T>(amountOfInputMaps, inputWidth, inputHeight, filter);

        return convolutionalLayer2D;
    }
}

#endif