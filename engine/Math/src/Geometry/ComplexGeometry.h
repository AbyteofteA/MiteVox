#ifndef COMPLEXGEOMETRY_H
#define COMPLEXGEOMETRY_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/GeometryPrimitiveBase.h"

namespace mathem
{
	enum struct GeometryType : uint8_t
	{
		POINT = 0,			/// 
		PRIMITIVES,			///
		MESH,				/// 
		SKELETON			/// 
	};

	class ComplexGeometry
	{
	public:

		GeometryType type = GeometryType::POINT;
		safety::SafeArray<GeometryPrimitiveBase*> primitives;
	};
}

#endif