
#ifndef FILTER_2D_SERIALIZER_H
#define FILTER_2D_SERIALIZER_H

#include "Filter2D.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace aimods
{
    template <typename T>
    fileio::JSON* Filter2DToJSON(Filter2D<T>* filter2D)
    {
        fileio::JSON* json = new fileio::JSON();

        // Serialize general filter's information.

        json->setField("dimension", (double)filter2D->getDimension());
        json->setField("stride", (double)filter2D->getStride());
        json->setField("paddingType", (double)filter2D->getPaddingType());
        json->setField("padding", (double)filter2D->getPadding());
        json->setField("fillType", (double)filter2D->getFillType());
        json->setField("amountOfKernels", (double)filter2D->getAmountOfKernels());
        json->setField("activationFunction", (double)filter2D->getActivationFunction());

        // Serialize filter's elements if there are any.

        T* filterData = filter2D->getFilterData();
        if (filterData != nullptr)
        {
            fileio::JSON* filterDataJSON = json->setFieldType("filterData", fileio::JSONtype::ARRAY);

            size_t filterElementCount = filter2D->computeFilterDataElementCount();
            for (size_t elementIndex = 0; elementIndex < filterElementCount; elementIndex++)
            {
                filterDataJSON->addArrayItem((double)filterData[elementIndex]);
            }
        }

        return json;
    }
}

#endif