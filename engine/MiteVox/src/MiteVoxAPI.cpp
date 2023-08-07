#include "MiteVoxAPI.h"

#include "engine/Renderer/src/RendererAPI/Camera.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/Math/src/Geometry/CollisionDetection/checkCollision.h"
#include "engine/MiteVox/src/Playground/Entity.h"
#include "engine/MiteVox/src/Playground/Scene.h"
#include "engine/MiteVox/src/Engine.h"

namespace mitevox
{
	Engine* MiteVoxAPI::engine = nullptr;

	void MiteVoxAPI::init(Engine* engine)
	{
		MiteVoxAPI::engine = engine;
	}

	EngineSettings* MiteVoxAPI::getSettings()
	{
		return engine->settings;
	}

	Entity* MiteVoxAPI::createEmptyEntity(std::string name)
	{
		Entity* entity = engine->useEntity();
		entity->name = name;
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
		engine->playground->addEntity(entity);

		return entity;
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
		engine->playground->addEntity(lightEntity);
		return lightEntity;
	}

	Entity* MiteVoxAPI::createMeshEntity(std::string name)
	{
		Entity* entity = MiteVoxAPI::createEmptyEntity(name);
		Node* renderableNode = entity->tryAttachNewRenderableNode();
		Mesh* mesh = renderableNode->tryAttachNewMesh();
		MeshPrimitive* meshPrimitive = mesh->addMeshPrimitive();
		meshPrimitive->initTriangles(12, true);
		return entity;
	}

	Entity* MiteVoxAPI::createBox(std::string name, float halfSizeX, float halfSizeY, float halfSizeZ, mathem::Vector3D position, render::ColorRGBAf color)
	{
		Entity* entity = MiteVoxAPI::createEmptyEntity(name);
		entity->transform.translation = position;
		
		//entity->movementProperties.restitution = 0.9f;

		Node* renderableNode = entity->tryAttachNewRenderableNode();
		Mesh* mesh = renderableNode->tryAttachNewMesh();
		MeshPrimitive* meshPrimitive = mesh->addMeshPrimitive();
		meshPrimitive->initTriangles(12, true);
		meshPrimitive->appendTopologyElements(12);
		meshPrimitive->material->baseColor = color;
		//meshPrimitive->material->illuminationModel = 0;

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
		auto triangle = meshPrimitive->getTrianglePositions(0);
		mathem::Vector3D normal = triangle.computeNormal();
		meshPrimitive->setTriangleNormals(0, normal, normal, normal);
		meshPrimitive->setTriangleNormals(1, normal, normal, normal);

		meshPrimitive->setTrianglePositions(2, cubeVertecesPositions[4], cubeVertecesPositions[6], cubeVertecesPositions[5]);
		meshPrimitive->setTrianglePositions(3, cubeVertecesPositions[4], cubeVertecesPositions[7], cubeVertecesPositions[6]);
		triangle = meshPrimitive->getTrianglePositions(2);
		normal = triangle.computeNormal();
		meshPrimitive->setTriangleNormals(2, normal, normal, normal);
		meshPrimitive->setTriangleNormals(3, normal, normal, normal);

		// XY

		meshPrimitive->setTrianglePositions(4, cubeVertecesPositions[0], cubeVertecesPositions[4], cubeVertecesPositions[5]);
		meshPrimitive->setTrianglePositions(5, cubeVertecesPositions[0], cubeVertecesPositions[5], cubeVertecesPositions[1]);
		triangle = meshPrimitive->getTrianglePositions(4);
		normal = triangle.computeNormal();
		meshPrimitive->setTriangleNormals(4, normal, normal, normal);
		meshPrimitive->setTriangleNormals(5, normal, normal, normal);

		meshPrimitive->setTrianglePositions(6, cubeVertecesPositions[3], cubeVertecesPositions[6], cubeVertecesPositions[7]);
		meshPrimitive->setTrianglePositions(7, cubeVertecesPositions[3], cubeVertecesPositions[2], cubeVertecesPositions[6]);
		triangle = meshPrimitive->getTrianglePositions(6);
		normal = triangle.computeNormal();
		meshPrimitive->setTriangleNormals(6, normal, normal, normal);
		meshPrimitive->setTriangleNormals(7, normal, normal, normal);

		// YZ

		meshPrimitive->setTrianglePositions(8, cubeVertecesPositions[0], cubeVertecesPositions[3], cubeVertecesPositions[7]);
		meshPrimitive->setTrianglePositions(9, cubeVertecesPositions[0], cubeVertecesPositions[7], cubeVertecesPositions[4]);
		triangle = meshPrimitive->getTrianglePositions(8);
		normal = triangle.computeNormal();
		meshPrimitive->setTriangleNormals(8, normal, normal, normal);
		meshPrimitive->setTriangleNormals(9, normal, normal, normal);

		meshPrimitive->setTrianglePositions(10, cubeVertecesPositions[1], cubeVertecesPositions[6], cubeVertecesPositions[2]);
		meshPrimitive->setTrianglePositions(11, cubeVertecesPositions[1], cubeVertecesPositions[5], cubeVertecesPositions[6]);
		triangle = meshPrimitive->getTrianglePositions(10);
		normal = triangle.computeNormal();
		meshPrimitive->setTriangleNormals(10, normal, normal, normal);
		meshPrimitive->setTriangleNormals(11, normal, normal, normal);

		entity->tryGenerateHitbox();
		entity->setMass(1.0f);

		engine->playground->addEntity(entity);

		return entity;
	}

	Entity* MiteVoxAPI::createCube(std::string name, mathem::Vector3D position, render::ColorRGBAf color)
	{
		Entity* entity = createBox(name, 0.5f, 0.5f, 0.5f, position, color);
		//entity->movementProperties.angularVelocity = { 0.0f, 3.14f, 0.0f };
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
		Entity* entity = MiteVoxAPI::createBox(name, 10.0f, 0.5f, 10.0f, { 0.0f, -7.0f, 0.0f }, color);
		entity->setMass(0.0f);
		return entity;
	}

	safety::SafeArray<Entity*>* MiteVoxAPI::collectEntitiesToSimulate()
	{
		Scene* activeScene = MiteVoxAPI::getActiveScene();
		engine->entitiesToSimulate.clear();
		if (engine->settings->spaceCulling)
		{
			// TODO:
			activeScene->foundation->getAll(&engine->entitiesToSimulate);
		}
		else
		{
			activeScene->foundation->getAll(&engine->entitiesToSimulate);
		}
		return &engine->entitiesToSimulate;
	}

	safety::SafeArray<mathem::CollisionInfo<Entity*>>* MiteVoxAPI::computeCollisions()
	{
		/*mathem::checkCollisions(
			&engine->entitiesToSimulate, 
			&engine->collisions, 
			engine->settings->getEqualityTolerance());*/

		Scene* activeScene = MiteVoxAPI::getActiveScene();
		activeScene->foundation->getCollisions(
			&engine->collisions, 
			&engine->dataPointsContainers, 
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