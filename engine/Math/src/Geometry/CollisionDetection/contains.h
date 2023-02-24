#ifndef CONTAINS_H
#define CONTAINS_H

#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"
#include "engine/Math/src/Geometry/ComplexGeometry.h"

namespace mathem
{
	bool contains(
		GeometryPrimitiveBase* geometryPrimitive,
		GeometryTransform* geometryPrimitiveTransform,
		GeometryPrimitiveBase* geometryPrimitiveContainer,
		GeometryTransform* geometryPrimitiveContainerTransform);

	bool contains(
		ComplexGeometry* complexGeometry,
		GeometryTransform* complexGeometryTransform,
		GeometryPrimitiveBase* geometryPrimitiveContainer,
		GeometryTransform* geometryPrimitiveContainerTransform);
}

#endif