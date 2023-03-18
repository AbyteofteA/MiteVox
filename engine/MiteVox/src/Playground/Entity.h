#ifndef MITEVOX_ENTITY_H
#define MITEVOX_ENTITY_H

#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/Physics/src/MovementProperties.h"
#include "engine/MiteVox/src/Playground/Node.h"

namespace mitevox
{
	class Entity
	{
    public:

        uint32_t ID = 0;
        mathem::GeometryTransform transform;
		mathem::ComplexGeometry collider;
		physcs::MovementProperties movementProperties;
		Node* renderableNode;

        mathem::GeometryTransform* getTransform();
        mathem::GeometryTransform* getTransformForMove();
        bool getIsMoved();
        void resetTransform();
        void setTransform(mathem::GeometryTransform transform);
        void setTranslation(mathem::Vector3D translation);
        void setRotation(mathem::Quaternion quaternion);
        void setRotation(mathem::Vector3D eulersRadians);
        void setScale(mathem::Vector3D scale);

        mathem::Matrix3x3 getInverseMomentOfInertia();

        mathem::ComplexGeometry* getCollider();
        void tryGenerateHitbox();

    private:

        bool isMoved = false;
	};
}

#endif

