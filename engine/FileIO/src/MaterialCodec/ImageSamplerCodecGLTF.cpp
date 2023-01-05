#include "ImageSamplerCodec.h"

namespace fileio
{
	void ImageSamplerCodec::fromGLTF(mitevox::ImageSampler* imageSamplerResult, JSON* imageSamplerJSON)
	{
		imageSamplerResult->name = imageSamplerJSON->getFieldStringOrDefault("name", "Untitled");

		if (JSON* numberJSON = imageSamplerJSON->getField("magFilter"))
		{
			imageSamplerResult->magnificationFilter =
				(mitevox::ImageSampler::FilterType)numberJSON->getNumberOrDefault((double)mitevox::ImageSampler::FilterType::LINEAR);
		}
		if (JSON* numberJSON = imageSamplerJSON->getField("minFilter"))
		{
			imageSamplerResult->minificationFilter =
				(mitevox::ImageSampler::FilterType)numberJSON->getNumberOrDefault((double)mitevox::ImageSampler::FilterType::LINEAR);
		}
		if (JSON* numberJSON = imageSamplerJSON->getField("wrapS"))
		{
			imageSamplerResult->wrappingModeU =
				(mitevox::ImageSampler::WrappingMode)numberJSON->getNumberOrDefault((double)mitevox::ImageSampler::WrappingMode::REPEAT);
		}
		if (JSON* numberJSON = imageSamplerJSON->getField("wrapT"))
		{
			imageSamplerResult->wrappingModeV =
				(mitevox::ImageSampler::WrappingMode)numberJSON->getNumberOrDefault((double)mitevox::ImageSampler::WrappingMode::REPEAT);
		}
	}
}