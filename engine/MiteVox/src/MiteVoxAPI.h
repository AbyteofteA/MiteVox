#ifndef MITEVOXAPI_H
#define MITEVOXAPI_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Renderer/src/RendererAPI/Color.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/MiteVox/src/EngineSettings.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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
		static mathem::Vector3D getRandomDirection();

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

		static float getCurrentDeltaTime();
		static PhysicsSolverType getPhysicsSolverType();
		static size_t getSubstepsCount();
		static float getSleepTime();
		static float getMaxLinearSpeed();
		static float getMaxAngularSpeed();
		static float getLinearDamping();
		static float getAngularDamping();
		static float getLinearSleepThreshold();
		static float getAngularSleepThreshold();
		static safety::SafeArray<Entity*>* collectEntitiesToSimulate();
		static safety::SafeArray<mathem::CollisionInfo<Entity*>>* computeCollisions();
		static mathem::Vector3D getGravity(Entity* entity);

		static void renderScene(
			render::RendererSettings* renderer,
			int shadowMapShaderID,
			int gBufferShaderID,
			int deferredLightingShaderID,
			int postprocessingShaderID,
			mathem::Vector3D ambientLight,
			safety::SafeArray<render::PointLight>* pointLightsArray,
			safety::SafeArray<render::DirectionalLight>* directionalLightsArray,
			safety::SafeArray<render::SpotLight>* spotLightsArray,
			render::Camera* camera,
			mathem::GeometryTransform* cameraTransform,
			safety::SafeArray<Entity*> entities,
			bool debug = false,
			int skyboxShaderID = -1,
			render::Cubemap* skybox = nullptr);

		static void renderSceneToGbuffer(
			render::RendererSettings* renderer,
			int shaderID,
			render::Camera* camera,
			mathem::GeometryTransform* cameraTransform,
			safety::SafeArray<Entity*> entities);

		static void renderSceneWithSpotLights(
			render::RendererSettings* renderer,
			int shadowMapShaderID,
			int lightingShaderID,
			safety::SafeArray<render::SpotLight>* spotLightsArray,
			safety::SafeArray<Entity*> entities,
			render::Camera* camera,
			mathem::GeometryTransform* cameraTransform);

		static void renderSceneWithPointLights(
			render::RendererSettings* renderer,
			int shadowMapShaderID,
			int lightingShaderID,
			safety::SafeArray<render::PointLight>* pointLightsArray,
			safety::SafeArray<Entity*> entities,
			render::Camera* camera,
			mathem::GeometryTransform* cameraTransform);

	private:

		static Engine* engine;
	};
}

#endif

