#ifndef GLTFIMAGESAMPLER_H
#define GLTFIMAGESAMPLER_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"

#include <string>

namespace fileio
{
	class ImageSampler
	{
	public:

		enum FilterType
		{
			NEAREST = 9728,
			LINEAR = 9729,
			NEAREST_MIPMAP_NEAREST = 9984,
			LINEAR_MIPMAP_NEAREST = 9985,
			NEAREST_MIPMAP_LINEAR = 9986,
			LINEAR_MIPMAP_LINEAR = 9987
		};

		enum WrappingMode
		{
			CLAMP_TO_EDGE = 33071,
			MIRRORED_REPEAT = 33648,
			REPEAT = 10497
		};

		FilterType magnificationFilter = FilterType::NEAREST;
		FilterType minificationFilter = FilterType::NEAREST;
		WrappingMode wrappingModeU = WrappingMode::REPEAT;
		WrappingMode wrappingModeV = WrappingMode::REPEAT;
		std::string name;

		void fromGLTF(JSON* imageSamplerJSON);
	};
}

#endif