#include "ResourceManager.h"

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/FileIO/src/Formats/Asset3DCodecGLTF/Asset3DCodecGLTF.h"

namespace mitevox
{
	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{

	}

	void ResourceManager::close()
	{
		size_t assets3DCount = assets3D.getElementsCount();
		for (size_t i = 0; i < assets3DCount; ++i)
		{
			mitevox::Asset3D* asset3D = assets3D.getElement(i).content;
			// TODO: save?
			delete asset3D;
		}
		assets3D.deallocate();

		// TODO: 
	}

	EngineSettings& ResourceManager::readConfig(EngineSettings& settings)
	{
		settingsPath = settings.getExecutableDir() + "\\engine_config.json";

		fileio::JSON* engineConfig = new fileio::JSON();
		engineConfig->readFromFile(settingsPath);
		settings.fromJSON(engineConfig);
		delete engineConfig;

		return settings;
	}

	void ResourceManager::saveConfig(EngineSettings& settings)
	{
		fileio::JSON* engineConfig = settings.toJSON();
		engineConfig->saveToFile(settingsPath);
		delete engineConfig;
	}

	void ResourceManager::readAssets3D(std::string asset3DPath)
	{
		Asset3DResource newAsset;
		mitevox::Asset3D* asset3D = new mitevox::Asset3D();
		fileio::fromGLTF(asset3DPath, asset3D);
		newAsset.content = asset3D;
		newAsset.path = asset3DPath;
		assets3D.appendElement(newAsset);
	}

	Asset3D* ResourceManager::getAssets3D(size_t index)
	{
		return assets3D.getElement(index).content;
	}
}