#include "MiteVoxAPI.h"

#include "engine/Renderer/src/RendererAPI/Camera.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkCollision.h"
#include "engine/Math/src/NumericalAnalysis/Intertolation.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/MiteVox/src/Engine.h"

#include <cstdlib>

namespace mitevox
{
	Engine* MiteVoxAPI::engine = nullptr;

	void MiteVoxAPI::init(Engine* engine)
	{
		MiteVoxAPI::engine = engine;
	}

	void MiteVoxAPI::setRandomSeed(size_t seed)
	{
		std::srand(seed);
	}

	size_t MiteVoxAPI::getRandom()
	{
		return std::rand();
	}

	size_t MiteVoxAPI::getRandom(size_t min, size_t max)
	{
		size_t randomFactor = (size_t)std::rand() / (size_t)RAND_MAX;
		return mathem::lerp<size_t>(min, max, randomFactor);
	}

	float MiteVoxAPI::getRandom(float min, float max)
	{
		float randomFactor = (float)std::rand() / (float)RAND_MAX;
		return mathem::lerp<float>(min, max, randomFactor);
	}

	mathem::Vector3D MiteVoxAPI::getRandomDirection()
	{
		mathem::Vector3D direction;
		direction.x() = MiteVoxAPI::getRandom(-1.0f, 1.0f);
		direction.y() = MiteVoxAPI::getRandom(-1.0f, 1.0f);
		direction.z() = MiteVoxAPI::getRandom(-1.0f, 1.0f);
		direction.normalize();
		return direction;
	}

	EngineSettings* MiteVoxAPI::getSettings()
	{
		return engine->settings;
	}

	Entity* MiteVoxAPI::createEmptyEntity(std::string name)
	{
		Entity* entity = engine->useEntity();
		entity->name = name;
		entity->collider.setType(mathem::GeometryType::POINT);
		engine->playground->addEntity(entity);
		return entity;
	}

	void MiteVoxAPI::deleteEntity(Entity* entity)
	{
		engine->returnEntity(entity);
	}

	Scene* MiteVoxAPI::getActiveScene()
	{
		return engine->playground->getActiveScene();
	}

	Entity* MiteVoxAPI::createFPSCharacter(std::string name)
	{
		Entity* entity = MiteVoxAPI::createEmptyEntity(name);
		entity->transform.translation = { 0.0f, 0.0f, 20.0f };
		entity->renderableNode = new Node();
		entity->renderableNode->camera = new render::Camera();
		entity->collider.setType(mathem::GeometryType::POINT);
		//entity->collider->tryMakeCapsule();

		//entity->characterController = new CharacterController(entity);
		entity->setMass(0.0f);

		MiteVoxAPI::getActiveScene()->activeCameraEntity = entity;

		return entity;
	}

	Entity* MiteVoxAPI::getEntity(std::string name)
	{
		Scene* activeScene = MiteVoxAPI::getActiveScene();
		size_t entitiesCount = activeScene->entities.getElementsCount();
		for (size_t i = 0; i < entitiesCount; ++i)
		{
			Entity* entity = activeScene->entities.getElement(i);
			if (entity->name == name)
			{
				return entity;
			}
		}
		return nullptr;
	}

	Entity* MiteVoxAPI::getActiveCameraEntity()
	{
		return MiteVoxAPI::getActiveScene()->activeCameraEntity;
	}

	Entity* MiteVoxAPI::createPointLightEntity(mathem::Vector3D position, render::ColorRGBf color, float intensity, float range)
	{
		Entity* lightEntity = MiteVoxAPI::createEmptyEntity("Point Light");
		lightEntity->renderableNode = new Node();
		lightEntity->renderableNode->lightType = render::LightType::POINT;
		lightEntity->renderableNode->light.pointLight.lightBase.color = color;
		lightEntity->renderableNode->light.pointLight.lightBase.intensity = intensity;
		lightEntity->renderableNode->light.pointLight.lightBase.range = range;
		lightEntity->setTranslation(position);
		lightEntity->collider.setType(mathem::GeometryType::POINT);
		return lightEntity;
	}

	Entity* MiteVoxAPI::createMeshEntity(std::string name)
	{
		Entity* entity = MiteVoxAPI::createEmptyEntity(name);
		Node* renderableNode = entity->tryAttachNewRenderableNode();
		Mesh* mesh = renderableNode->tryAttachNewMesh();
		MeshPrimitive* meshPrimitive = mesh->addMeshPrimitive();
		return entity;
	}

	Entity* MiteVoxAPI::createBox(std::string name, float halfSizeX, float halfSizeY, float halfSizeZ, mathem::Vector3D position, render::ColorRGBAf color)
	{
		Entity* entity = MiteVoxAPI::createMeshEntity(name);
		entity->transform.translation = position;

		MeshPrimitive* meshPrimitive = entity->renderableNode->getMeshToRender()->primitives[0];
		meshPrimitive->initTriangles(12, true);
		meshPrimitive->appendTopologyElements(12);
		meshPrimitive->material->baseColor = color;
		
		const mathem::Vector3D cubeVertecesPositions[8] =
		{
			{-halfSizeX, -halfSizeY, -halfSizeZ}, 
			{halfSizeX, -halfSizeY, -halfSizeZ}, 
			{halfSizeX, -halfSizeY, halfSizeZ}, 
			{-halfSizeX, -halfSizeY, halfSizeZ},
			{-halfSizeX, halfSizeY, -halfSizeZ}, 
			{halfSizeX, halfSizeY, -halfSizeZ}, 
			{halfSizeX, halfSizeY, halfSizeZ}, 
			{-halfSizeX, halfSizeY, halfSizeZ}
		};

		// XZ

		meshPrimitive->setTrianglePositions(0, cubeVertecesPositions[0], cubeVertecesPositions[1], cubeVertecesPositions[2]);
		meshPrimitive->setTrianglePositions(1, cubeVertecesPositions[0], cubeVertecesPositions[2], cubeVertecesPositions[3]);
		meshPrimitive->setTrianglePositions(2, cubeVertecesPositions[4], cubeVertecesPositions[6], cubeVertecesPositions[5]);
		meshPrimitive->setTrianglePositions(3, cubeVertecesPositions[4], cubeVertecesPositions[7], cubeVertecesPositions[6]);

		// XY

		meshPrimitive->setTrianglePositions(4, cubeVertecesPositions[0], cubeVertecesPositions[4], cubeVertecesPositions[5]);
		meshPrimitive->setTrianglePositions(5, cubeVertecesPositions[0], cubeVertecesPositions[5], cubeVertecesPositions[1]);
		meshPrimitive->setTrianglePositions(6, cubeVertecesPositions[3], cubeVertecesPositions[6], cubeVertecesPositions[7]);
		meshPrimitive->setTrianglePositions(7, cubeVertecesPositions[3], cubeVertecesPositions[2], cubeVertecesPositions[6]);

		// YZ

		meshPrimitive->setTrianglePositions(8, cubeVertecesPositions[0], cubeVertecesPositions[3], cubeVertecesPositions[7]);
		meshPrimitive->setTrianglePositions(9, cubeVertecesPositions[0], cubeVertecesPositions[7], cubeVertecesPositions[4]);
		meshPrimitive->setTrianglePositions(10, cubeVertecesPositions[1], cubeVertecesPositions[6], cubeVertecesPositions[2]);
		meshPrimitive->setTrianglePositions(11, cubeVertecesPositions[1], cubeVertecesPositions[5], cubeVertecesPositions[6]);

		meshPrimitive->tryGenerateFlatNormals();

		entity->tryGenerateHitbox();
		entity->physicalMaterial.setRestitution(0.0f);
		entity->physicalMaterial.setDynamicFriction(1.0f);
		entity->physicalMaterial.setStaticFriction(1.0f);
		entity->computeMass();

		return entity;
	}

	Entity* MiteVoxAPI::createCube(std::string name, mathem::Vector3D position, render::ColorRGBAf color)
	{
		Entity* entity = createBox(name, 0.5f, 0.5f, 0.5f, position, color);
		return entity;
	}

	Entity* MiteVoxAPI::createFrozenCube(std::string name, mathem::Vector3D position, render::ColorRGBAf color)
	{
		Entity* entity = MiteVoxAPI::createBox(name, 0.5f, 0.5f, 0.5f, position, color);
		entity->setMass(0.0f);
		return entity;
	}

	Entity* MiteVoxAPI::createPlane(std::string name, render::ColorRGBAf color)
	{
		Entity* entity = MiteVoxAPI::createBox(name, 10.0f, 1.5f, 10.0f, { 0.0f, -1.5f, 0.0f }, color);
		entity->setMass(0.0f);
		return entity;
	}

	float MiteVoxAPI::getCurrentDeltaTime()
	{
		return engine->deltaTime / (float)engine->settings->physics.substepsCount;
	}

	PhysicsSolverType MiteVoxAPI::getPhysicsSolverType()
	{
		return engine->settings->physics.physicsSolver;
	}

	size_t MiteVoxAPI::getSubstepsCount()
	{
		return engine->settings->physics.substepsCount;
	}

	float MiteVoxAPI::getSleepTime()
	{
		return engine->settings->physics.sleepTime;
	}

	float MiteVoxAPI::getMaxLinearSpeed()
	{
		return engine->settings->physics.maxLinearSpeed;
	}

	float MiteVoxAPI::getMaxAngularSpeed()
	{
		return engine->settings->physics.maxAngularSpeed;
	}

	float MiteVoxAPI::getLinearDamping()
	{
		return engine->settings->physics.linearDamping;
	}

	float MiteVoxAPI::getAngularDamping()
	{
		return engine->settings->physics.angularDamping;
	}

	float MiteVoxAPI::getLinearSleepThreshold()
	{
		return engine->settings->physics.linearSleepThreshold;
	}

	float MiteVoxAPI::getAngularSleepThreshold()
	{
		return engine->settings->physics.angularSleepThreshold;
	}

	safety::SafeArray<Entity*>* MiteVoxAPI::collectEntitiesToSimulate()
	{
		Scene* activeScene = MiteVoxAPI::getActiveScene();
		engine->entitiesToSimulate.clear();
		engine->entitiesToSimulate.concatenate(&activeScene->entities);
		return &engine->entitiesToSimulate;
	}

	safety::SafeArray<mathem::CollisionInfo<Entity*>>* MiteVoxAPI::computeCollisions()
	{
		mathem::checkCollisions(
			&engine->entitiesToSimulate, 
			&engine->collisions, 
			engine->settings->getEqualityTolerance());

		return &engine->collisions;
	}

	mathem::Vector3D MiteVoxAPI::getGravity(Entity* entity)
	{
		Scene* activeScene = MiteVoxAPI::getActiveScene();
		if (activeScene->isGravityGlobal)
		{
			return activeScene->globalGravity;
		}
		else
		{
			// TODO: return scene->computeLocalGravity();
		}
		return activeScene->globalGravity;
	}
}