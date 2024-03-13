#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include "engine/MiteVox/src/BufferLayout/BufferView.h"
#include "engine/MiteVox/src/BufferLayout/BufferViewAccessor.h"
#include "engine/MiteVox/src/Material/ImageSampler.h"
#include "engine/MiteVox/src/Material/Image.h"
#include "engine/MiteVox/src/Material/Texture.h"
#include "engine/MiteVox/src/Material/Material.h"
#include "engine/MiteVox/src/Physics/PhysicalMaterial.h"
#include "engine/MiteVox/src/Mesh/MeshPrimitive.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/MiteVox/src/Animation/Animation.h"
#include "engine/MiteVox/src/Skeleton/SkeletonBase.h"
#include "engine/FileIO/src/Formats/FileFormatBase.h"
#include "engine/FileIO/src/SerializationStatus.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Renderer/src/RendererAPI/Camera.h"

#include <string>

namespace mitevox
{
	/// <summary>
	/// 
	/// TODO: implement serialization to glTF, CBOR
	/// TODO: implement deserialization from CBOR
	/// </summary>
	class Asset3D
	{
	public:

		std::string name = "Untitled";
		size_t activeScene = 0;

		Asset3D(std::string _name = "Untitled");
		~Asset3D();

		void updateAssets();

		size_t createDefaultScene();
		size_t createScene(std::string name);
		size_t createActiveScene(std::string name);
		void deleteScene(size_t index);
		Scene* getActiveScene();

		size_t addEntity(Entity* entity);
		void removeEntity(Entity* entity);

	// TODO: make private:

		struct AssetInfo
		{
			std::string version;
			std::string minVersion;
			std::string generator;
			std::string copyright;
		} assetInfo;

		safety::SafeArray<std::string> extensionsUsed;
		safety::SafeArray<std::string> extensionsRequired;

		safety::SafeArray<safety::SafeByteArray*> buffers;
		safety::SafeArray<BufferView*> bufferViews;
		safety::SafeArray<BufferViewAccessor*> accessors;
		safety::SafeArray<render::Camera*> cameras;
		safety::SafeArray<ImageSampler> imageSamplers;
		safety::SafeArray<Image> images;
		safety::SafeArray<Texture> textures;
		safety::SafeArray<Material> materials;
		safety::SafeArray<PhysicalMaterial> physicalMaterials;
		safety::SafeArray<Mesh*> meshes;
		safety::SafeArray<Node*> nodes;
		safety::SafeArray<Scene*> scenes;
		safety::SafeArray<Animation*> animations;
		safety::SafeArray<SkeletonBase*> skeletons;
	};
}

#endif