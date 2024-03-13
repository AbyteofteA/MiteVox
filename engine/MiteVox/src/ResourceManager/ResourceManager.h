#ifndef MITEVOX_RESOURCEMANAGER_H
#define MITEVOX_RESOURCEMANAGER_H

#include "Resource.h"
#include "engine/MiteVox/src/EngineSettings.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/MiteVox/src/Playground/Asset3D.h"
#include "engine/MiteVox/src/Material/Image.h"

#include <vector>
#include <string>

namespace mitevox
{
	typedef Resource<Asset3D*> Asset3DResource;
	//typedef Resource<Image*> ImageResource;
	//typedef Resource<> TextResource;
	//typedef Resource<> ShaderResource;
	//typedef Resource<> GameDataResource;
	//typedef Resource<> GameSaveResource;

	class ResourceManager
	{
	public:

		ResourceManager();
		~ResourceManager();

		void close();

		EngineSettings& readConfig(EngineSettings& settings);
		void saveConfig(EngineSettings& settings);

		void readAssets3D(std::string asset3DPath);
		Asset3D* getAssets3D(size_t index);

	private:

		std::string executableDir;
		std::string settingsPath;
		safety::SafeArray<Asset3DResource> assets3D;
	};
}

#endif