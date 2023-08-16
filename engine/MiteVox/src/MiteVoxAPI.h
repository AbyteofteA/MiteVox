#ifndef MITEVOXAPI_H
#define MITEVOXAPI_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Renderer/src/RendererAPI/Color.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/MiteVox/src/EngineSettings.h"

#include <string>

namespace mitevox
{
	class Entity;
	class Scene;
	class Engine;

	class MiteVoxAPI
	{
	public:

		static void init(Engine* engine);

		// Random

		static void setRandomSeed(size_t seed);
		static size_t getRandom();
		static size_t getRandom(size_t min, size_t max);
		static float getRandom(float min, float max);

		// Queries

		static EngineSettings* getSettings();
		static Scene* getActiveScene();
		static Entity* getEntity(std::string name);
		static Entity* getActiveCameraEntity();
		//static CharacterController* getActiveCharacterController();

		// Entity creation

		static Entity* createEmptyEntity(std::string name);
		static void deleteEntity(Entity* entity);

		/// <summary>
		/// Creates an entity with the following properties:
		/// * Perspective camera
		/// * Capsule collider
		/// * Kinematic character controller
		/// </summary>
		/// <returns></returns>
		static Entity* createFPSCharacter(std::string name);

		static Entity* createPointLightEntity(mathem::Vector3D position, render::ColorRGBf color, float intensity = 1.0f, float range = 40.0f);

		/// <summary>
		/// Creates an entity with the following properties:
		/// * Empty mesh
		/// </summary>
		/// <returns></returns>
		static Entity* createMeshEntity(std::string name);

		static Entity* createBox(std::string name, float halfSizeX, float halfSizeY, float halfSizeZ, mathem::Vector3D position, render::ColorRGBAf color);
		static Entity* createCube(std::string name, mathem::Vector3D position, render::ColorRGBAf color);
		static Entity* createFrozenCube(std::string name, mathem::Vector3D position, render::ColorRGBAf color);
		static Entity* createPlane(std::string name, render::ColorRGBAf color);

		// Physics

		static safety::SafeArray<Entity*>* collectEntitiesToSimulate();
		static safety::SafeArray<mathem::CollisionInfo<Entity*>>* computeCollisions();
		static mathem::Vector3D getGravity(Entity* entity);

		// Rendering

		static void renderNodeRecursively(
			int shaderID,
			Node* node,
			mathem::GeometryTransform* nodeTransform,
			render::Camera* camera,
			mathem::GeometryTransform* cameraTransform);
		static void removeNodeRecursively(int shaderID, Node* node);

	private:

		static Engine* engine;
	};
}

#endif

