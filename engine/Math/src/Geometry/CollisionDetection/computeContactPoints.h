#ifndef COMPUTECONTACTPOINTS_H
#define COMPUTECONTACTPOINTS_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/SphereGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/CapsuleGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TruncatedPyramidGeometry.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	void computeContactPointsWithMesh(
		GeometryPrimitiveBase* mesh,
		GeometryTransform* meshTransform,
		GeometryPrimitiveBase* otherGeometry,
		GeometryTransform* otherGeometryTransform,
		CollisionProperties* collisionProperties,
		float equalityTolerance);

	void computeContactPoints(
		GeometryPrimitiveBase* geometryPrimitive1,
		GeometryTransform* geometryPrimitive1Transform,
		GeometryPrimitiveBase* geometryPrimitive2,
		GeometryTransform* geometryPrimitive2Transform,
		CollisionProperties* collisionProperties,
		float equalityTolerance);

	template <class T>
	inline void computeContactPoints(CollisionInfo<T>* collisionInfo, float equalityTolerance)
	{
		ComplexGeometry* complexGeometry1 = collisionInfo->object1->getCollider();
		ComplexGeometry* complexGeometry2 = collisionInfo->object2->getCollider();
		if (complexGeometry1->getType() == mathem::GeometryType::NONE ||
			complexGeometry2->getType() == mathem::GeometryType::NONE)
		{
			collisionInfo->reset();
			return;
		}

		GeometryTransform* complexGeometryTransform1 = collisionInfo->object1->getResultTransform();
		GeometryTransform* complexGeometryTransform2 = collisionInfo->object2->getResultTransform();

		collisionInfo->properties.resetContactPoints();

		size_t objectPrimitivesCount1 = complexGeometry1->getPrimitivesCount();
		for (size_t primitiveIndex1 = 0; primitiveIndex1 < objectPrimitivesCount1; primitiveIndex1++)
		{
			GeometryPrimitiveBase* geometryPrimitive1 = complexGeometry1->getPrimitive(primitiveIndex1);
			size_t objectPrimitivesCount2 = complexGeometry2->getPrimitivesCount();
			for (size_t primitiveIndex2 = 0; primitiveIndex2 < objectPrimitivesCount2; primitiveIndex2++)
			{
				GeometryPrimitiveBase* geometryPrimitive2 = complexGeometry2->getPrimitive(primitiveIndex2);
				computeContactPoints(
					geometryPrimitive1,
					complexGeometryTransform1,
					geometryPrimitive2,
					complexGeometryTransform2,
					&collisionInfo->properties,
					equalityTolerance);
			}
		}

		// Transform contact points to local space
		for (size_t i = 0; i < collisionInfo->properties.contactPointsCount; i++)
		{
			Vector3D object1Contact = collisionInfo->properties.contacts[i].contactPoints1;
			Vector3D object2Contact = collisionInfo->properties.contacts[i].contactPoints2;
			complexGeometryTransform1->applyInverseTo(object1Contact);
			complexGeometryTransform2->applyInverseTo(object2Contact);
			collisionInfo->properties.contacts[i].contactPoints1 = object1Contact;
			collisionInfo->properties.contacts[i].contactPoints2 = object2Contact;
		}
	}
}

#endif