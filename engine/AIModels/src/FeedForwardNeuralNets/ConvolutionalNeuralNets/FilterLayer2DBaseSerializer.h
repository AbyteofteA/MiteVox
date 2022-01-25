
#ifndef FILTERLAYER2DBASESERIALIZER_H
#define FILTERLAYER2DBASESERIALIZER_H

#include "FilterLayer2DBase.h"
#include "Filter2DSerializer.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBaseSerializer.h"

namespace aimods
{
    template <typename T>
    fileio::JSON* FilterLayer2DBaseToJSON(FilterLayer2DBase<T>* filterLayer2DBase)
    {
        fileio::JSON* json = new fileio::JSON();

        // Serialize general FilterLayer2DBase's information.

        json->setField("amountOfInputMaps", (double)filterLayer2DBase->getAmountOfInputMaps());
        json->setField("inputWidth", (double)filterLayer2DBase->getInputWidth());
        json->setField("inputHeight", (double)filterLayer2DBase->getInputHeight());

        // Serialize FilterLayer2DBase's filter elements.

        fileio::JSON* filter = Filter2DToJSON<T>(filterLayer2DBase->_filter);
        filter->setName("filter");
        json->setField(filter);

        return json;
    }
}

#endif
