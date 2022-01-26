#include "ImageSampler.h"

namespace fileio
{
	void ImageSampler::fromGLTF(JSON* imageSamplerJSON)
	{
		name = imageSamplerJSON->getFieldString("name");

		JSON* numberJSON = imageSamplerJSON->getField("magFilter");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				magnificationFilter = (ImageSampler::FilterType)numberJSON->getNumber();
			}
		}
		numberJSON = imageSamplerJSON->getField("minFilter");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				minificationFilter = (ImageSampler::FilterType)numberJSON->getNumber();
			}
		}
		numberJSON = imageSamplerJSON->getField("wrapS");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				wrappingModeU = (ImageSampler::WrappingMode)numberJSON->getNumber();
			}
		}
		numberJSON = imageSamplerJSON->getField("wrapT");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				wrappingModeV = (ImageSampler::WrappingMode)numberJSON->getNumber();
			}
		}
	}
}