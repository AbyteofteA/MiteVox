#ifndef FILTER2DCODEC_H
#define FILTER2DCODEC_H

#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"

namespace aimods
{
    class Filter2DCodec
    {
    public:

        template <typename T>
        static Filter2D<T>* fromJSON(fileio::JSON* filter2DJSON);

        template <typename T>
        static fileio::JSON* toJSON(Filter2D<T>* filter2D);
    };

    template <typename T>
    Filter2D<T>* Filter2DCodec::fromJSON(fileio::JSON* filter2DJSON)
    {
        size_t dimension = (size_t)filter2DJSON->getFieldNumberOrDefault("dimension", 0.0f);
        size_t stride = (size_t)filter2DJSON->getFieldNumberOrDefault("stride", 0.0f);
        PaddingType paddingType = (PaddingType)filter2DJSON->getFieldNumberOrDefault("paddingType", 0.0f);
        size_t padding = (size_t)filter2DJSON->getFieldNumberOrDefault("padding", 0.0f);
        FillType fillType = (FillType)filter2DJSON->getFieldNumberOrDefault("fillType", 0.0f);
        size_t amountOfKernels = (size_t)filter2DJSON->getFieldNumberOrDefault("amountOfKernels", 0.0f);

        Filter2D<T>* filter2D = nullptr;

        fileio::JSON* filterDataJSON = filter2DJSON->getFieldArray("filterData");
        if (filterDataJSON == nullptr)
        {
            filter2D = new Filter2D<T>(dimension, stride, fillType, paddingType, padding);
        }
        else
        {
            filter2D = new Filter2D<T>(amountOfKernels, dimension, stride, fillType, paddingType, padding);
            filterDataJSON->toNumberArray(&filter2D->filterData);
        }

        return filter2D;
    }

    template <typename T>
    fileio::JSON* Filter2DCodec::toJSON(Filter2D<T>* filter2D)
    {
        fileio::JSON* filter2DJSON = new fileio::JSON();
        filter2DJSON->setName("filter");

        filter2DJSON->setField("dimension", (double)filter2D->getDimension());
        filter2DJSON->setField("stride", (double)filter2D->getStride());
        filter2DJSON->setField("paddingType", (double)filter2D->getPaddingType());
        filter2DJSON->setField("padding", (double)filter2D->getPadding());
        filter2DJSON->setField("fillType", (double)filter2D->getFillType());
        filter2DJSON->setField("amountOfKernels", (double)filter2D->getAmountOfKernels());

        if (filter2D->filterData.getElementsCount() != 0)
        {
            fileio::JSON* filterDataJSON = filter2DJSON->setFieldType("filterData", fileio::JSONtype::ARRAY);
            filterDataJSON->fromNumberArray(&filter2D->filterData);
        }

        return filter2DJSON;
    }
}

#endif