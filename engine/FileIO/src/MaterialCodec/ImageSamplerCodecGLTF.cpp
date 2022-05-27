#include "ImageSamplerCodec.h"

namespace fileio
{
	void ImageSamplerCodec::fromGLTF(mitevox::ImageSampler* imageSamplerResult, JSON* imageSamplerJSON)
	{
		imageSamplerResult->name = imageSamplerJSON->getFieldString("name");

		JSON* numberJSON = imageSamplerJSON->getField("magFilter");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				imageSamplerResult->magnificationFilter =
					(mitevox::ImageSampler::FilterType)numberJSON->getNumber();
			}
		}
		numberJSON = imageSamplerJSON->getField("minFilter");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				imageSamplerResult->minificationFilter =
					(mitevox::ImageSampler::FilterType)numberJSON->getNumber();
			}
		}
		numberJSON = imageSamplerJSON->getField("wrapS");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				imageSamplerResult->wrappingModeU =
					(mitevox::ImageSampler::WrappingMode)numberJSON->getNumber();
			}
		}
		numberJSON = imageSamplerJSON->getField("wrapT");
		if (numberJSON != nullptr)
		{
			if (numberJSON->isNumber())
			{
				imageSamplerResult->wrappingModeV =
					(mitevox::ImageSampler::WrappingMode)numberJSON->getNumber();
			}
		}
	}
}