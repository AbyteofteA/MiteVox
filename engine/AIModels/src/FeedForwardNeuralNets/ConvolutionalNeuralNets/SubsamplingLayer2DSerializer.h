
#ifndef SUBSAMPLINGLAYER2DSERIALIZER_H
#define SUBSAMPLINGLAYER2DSERIALIZER_H

#include "SubsamplingLayer2D.h"
#include "FilterLayer2DBase.h"
#include "FilterLayer2DBaseSerializer.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace aimods
{
    template <typename T>
    fileio::JSON* SubsamplingLayer2DToJSON(SubsamplingLayer2D<T>* subsamplingLayer2D)
    {
        fileio::JSON* json = new fileio::JSON();

        fileio::JSON* neuralNetworkLayerBase =
            NeuralNetworkLayerBaseToJSON<T>((NeuralNetworkLayerBase<T>*)subsamplingLayer2D);
        neuralNetworkLayerBase->setName("NeuralNetworkLayerBase");
        json->setField(neuralNetworkLayerBase);

        fileio::JSON* filterLayer2DBase = FilterLayer2DBaseToJSON<T>((FilterLayer2DBase<T>*)subsamplingLayer2D);
        filterLayer2DBase->setName("FilterLayer2DBase");
        json->setField(filterLayer2DBase);

        fileio::JSON* subsamplingLayer2DJSON = json->setFieldType("SubsamplingLayer2D", fileio::JSONtype::OBJECT);
        subsamplingLayer2DJSON->setField("weight", subsamplingLayer2D->getWeight());
        subsamplingLayer2DJSON->setField("threshold", subsamplingLayer2D->getThreshold());
        subsamplingLayer2DJSON->setField("function", (double)subsamplingLayer2D->getFunction());

        return json;
    }
}

#endif