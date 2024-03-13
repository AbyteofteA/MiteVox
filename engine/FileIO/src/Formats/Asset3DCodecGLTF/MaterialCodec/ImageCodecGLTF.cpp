#include "ImageCodec.h"

#include <atomic>
#include <string>
#include "engine/FileIO/src/FileStatus.h"
#include "engine/FileIO/src/FileInputOutput.h"
#include "engine/MiteVox/src/Material/Image.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace fileio
{
	void ImageCodec::fromGLTF(mitevox::Image* imageResult, JSON* imageJSON, std::string JSONPath)
	{
		imageResult->name = imageJSON->getFieldStringOrDefault("name", "Untitled");

		if (JSON* imageURIJSON = imageJSON->getField("uri"))
		{
			std::string uri = imageURIJSON->getStringOrDefault("ERROR");
			if (fs::path(uri).is_absolute() == false)
			{
				uri = fs::path(JSONPath).parent_path().string() + "\\" + uri;
			}
			FileInputOutput::loadImage(uri, imageResult);
		}
		else
		{
			std::string mimeType = imageJSON->getFieldStringOrDefault("mimeType", "ERROR");
			if (JSON* numberJSON = imageJSON->getField("bufferView"))
			{
				imageResult->bufferViewIndex = (int32_t)numberJSON->getNumberOrDefault(-1.0f);
			}
		}
	}

	void ImageCodec::toGLTF(JSON* imageJSONResult, mitevox::Image* image, size_t index, std::string JSONPath)
	{
		imageJSONResult->setField("name", image->name);

		if (image->bufferViewIndex == -1)
		{
			if (1/*image->filePath.empty()*/)
			{
				//image->filePath = JSONPath + "image" + std::to_string(index);
				image->filePath = "image" + std::to_string(index); // TODO: 
				image->filePath += ".png";
			}
			imageJSONResult->setField("uri", image->filePath);
			FileInputOutput::saveImage(image, image->filePath);
		}
		else
		{
			imageJSONResult->setField("mimeType", image->mimeType);
			imageJSONResult->setField("bufferView", (double)image->bufferViewIndex);
		}
	}
}