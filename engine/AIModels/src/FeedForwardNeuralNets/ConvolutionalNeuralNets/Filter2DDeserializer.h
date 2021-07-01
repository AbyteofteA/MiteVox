
#ifndef FILTER_2D_DESERIALIZER_H
#define FILTER_2D_DESERIALIZER_H

#include "Filter2D.h"
#include "engine/AIModels/src/Structure/Functions.h"
#include "engine/FileIO/src/Formats/JSON.h"

namespace aimods
{
    template <typename T>
    Filter2D<T>* Filter2DFromJSON(fileio::JSON* filter2DJSON)
    {
        // Deserialize general filter's information.

        size_t dimension = (size_t)filter2DJSON->getFieldNumber("dimension");
        size_t stride = (size_t)filter2DJSON->getFieldNumber("stride");
        PaddingType paddingType = (PaddingType)filter2DJSON->getFieldNumber("paddingType");
        size_t padding = (size_t)filter2DJSON->getFieldNumber("padding");
        FillType fillType = (FillType)filter2DJSON->getFieldNumber("fillType");
        size_t amountOfKernels = (size_t)filter2DJSON->getFieldNumber("amountOfKernels");
        ActivationFunction activationFunction = (ActivationFunction)filter2DJSON->getFieldNumber("activationFunction");

        // Deserialize filter's elements if there are any.

        Filter2D<T>* filter2D = nullptr;

        fileio::JSON* filterDataJSON = filter2DJSON->getFieldArray("filterData");
        if (filterDataJSON == nullptr)
        {
            filter2D = new Filter2D<T>(dimension, stride, activationFunction, fillType, paddingType, padding);
        }
        else
        {
            filter2D = new Filter2D<T>(amountOfKernels, dimension, stride, activationFunction, fillType, paddingType, padding);
            T* filterData = filter2D->getFilterData();

            size_t filterElementCount = filter2D->computeFilterDataElementCount();
            for (size_t elementIndex = 0; elementIndex < filterElementCount; elementIndex++)
            {
                filterData[elementIndex] = (T)filterDataJSON->getArrayItemNumber(elementIndex);
            }
        }

        return filter2D;
    }
}

#endif