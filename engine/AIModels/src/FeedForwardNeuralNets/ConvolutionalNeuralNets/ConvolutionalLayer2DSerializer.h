
#ifndef CONVOLUTIONALLAYER2DSERIALIZER_H
#define CONVOLUTIONALLAYER2DSERIALIZER_H

#include "ConvolutionalLayer2D.h"
#include "FilterLayer2DBaseSerializer.h"
#include "engine/FileIO/src/Formats/JSON.h"

namespace aimods
{
    template <typename T>
    fileio::JSON* ConvolutionalLayer2DToJSON(ConvolutionalLayer2D<T>* convolutionalLayer2D)
    {
        fileio::JSON* json = new fileio::JSON();

        fileio::JSON* neuralNetworkLayerBase = 
            NeuralNetworkLayerBaseToJSON<T>((NeuralNetworkLayerBase<T>*)convolutionalLayer2D);
        neuralNetworkLayerBase->setName("NeuralNetworkLayerBase");
        json->setField(neuralNetworkLayerBase);

        fileio::JSON* filterLayer2DBase = FilterLayer2DBaseToJSON<T>((FilterLayer2DBase<T>*)convolutionalLayer2D);
        filterLayer2DBase->setName("FilterLayer2DBase");
        json->setField(filterLayer2DBase);

        return json;
    }
}

#endif