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
#include "engine/Math/src/Geometry/GeometryPrimitives/RayGeometry.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"
#include "engine/MiteVox/src/Mesh/Mesh.h"
#include "engine/Math/src/Geometry/CollisionDetection/CollisionInfo.h"
#include "engine/Math/src/Vector.h"

namespace mathem
{
	void computeContactPoints(
		BoxGeometry* box,
		GeometryTransform* boxTransform,
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
		GeometryTransform* complexGeometryTransform1 = collisionInfo->object1->getTransform();
		ComplexGeometry* complexGeometry2 = collisionInfo->object2->getCollider();
		GeometryTransform* complexGeometryTransform2 = collisionInfo->object2->getTransform();

		collisionInfo->properties.resetContactPoints();

		size_t objectPrimitivesCount1 = complexGeometry1->primitives.getElementsCount();
		for (size_t primitiveIndex1 = 0; primitiveIndex1 < objectPrimitivesCount1; primitiveIndex1++)
		{
			GeometryPrimitiveBase* geometryPrimitive1 = complexGeometry1->primitives.getElement(primitiveIndex1);
			size_t objectPrimitivesCount2 = complexGeometry2->primitives.getElementsCount();
			for (size_t primitiveIndex2 = 0; primitiveIndex2 < objectPrimitivesCount2; primitiveIndex2++)
			{
				GeometryPrimitiveBase* geometryPrimitive2 = complexGeometry2->primitives.getElement(primitiveIndex2);
				computeContactPoints(
					geometryPrimitive1,
					complexGeometryTransform1,
					geometryPrimitive2,
					complexGeometryTransform2,
					&collisionInfo->properties,
					equalityTolerance);
			}
		}
	}
}

#endif