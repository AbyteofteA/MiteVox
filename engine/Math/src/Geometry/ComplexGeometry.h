
#ifndef COMPLEXGEOMETRY_H
#define COMPLEXGEOMETRY_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitiveBase.h"

namespace mathem
{
	class ComplexGeometry : public GeometryTransform
	{
		safety::SafeArray<GeometryPrimitiveBase*> primitives;
	};
}

#endif