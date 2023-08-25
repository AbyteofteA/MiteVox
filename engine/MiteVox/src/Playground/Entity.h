#ifndef MITEVOX_ENTITY_H
#define MITEVOX_ENTITY_H

#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/MiteVox/src/Playground/Node.h"
#include "engine/MiteVox/src/Animation/Animation.h"
#include "engine/MiteVox/src/Physics/PhysicalMaterial.h"
#include "engine/MiteVox/src/Physics/MovementProperties.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <cstdint>
#include <string>

namespace mitevox
{
	class Entity
	{
    public:

        std::string name;

        mathem::GeometryTransform transform;
        MovementProperties movementProperties;
        PhysicalMaterial physicalMaterial; // TOOD: make a reference
		mathem::ComplexGeometry collider;
        float timeUntilSleep = 1.0f;

		Node* renderableNode = nullptr;
        // TODO: instanceVariations;
        
        // TODO: animations;
        // TODO: animationState;
        // TODO: animationTable;

        // TODO: characterController;
        // TODO: ai;

        Entity();

        bool hasCamera();
        mathem::GeometryTransform getCamera(render::Camera** cameraResult);

        /// <summary>
        /// TODO: make it point from the camera
        /// </summary>
        mathem::Vector3D getViewRay();

        mathem::GeometryTransform* getTransform();
        void resetTransform();
        void setTransform(mathem::GeometryTransform transform);
        void setTranslation(mathem::Vector3D translation);
        void setRotation(mathem::Quaternion quaternion);
        void setRotation(mathem::Vector3D eulersRadians);
        void setScale(mathem::Vector3D scale);

        /// <summary>
        /// Computes mass based on density and volume. Setting the mass must the last thing in the setup of an Entity
        /// </summary>
        void computeMass();

        /// <summary>
        /// Setting the mass must the last thing in the setup of an Entity
        /// </summary>
        void setMass(float mass);

        float getMass();

        void computeMomentOfInertia();
        mathem::Matrix3x3 getMomentOfInertia();
        mathem::Matrix3x3 getInverseMomentOfInertia();

        mathem::ComplexGeometry* getCollider();
        void tryGenerateHitbox();

        void awake(float deltaTime);
        bool isSleeping();
        void updateSleeping(float deltaTime);
        void applyDamping(float deltaTime);
        void applyForce(mathem::Vector3D force);
        void applyForceAtPoint(mathem::Vector3D force, mathem::Vector3D point);
        void integrateForces(float deltaTime);
        void integrateGravity(mathem::Vector3D gravity, float deltaTime);
        void integrateVelocities(float deltaTime);
        void resetForces();

        Node* tryAttachNewRenderableNode();

    private:

        void computeMomentOfInertiaForPrimitives();
        void computeMomentOfInertiaForMesh();
        void computeMomentOfInertiaForSkeleton();
	};
}

#endif

