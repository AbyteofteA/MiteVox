
#ifndef GEOMETRYPRIMITIVEBASE_H
#define GEOMETRYPRIMITIVEBASE_H

#include "engine/Math/src/Point3D.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"

#include <cstdint>

namespace mathem
{
	enum struct GeometryPrimitiveType : uint8_t 
	{ 
		NONE = 0, 		   /// 
		BOX, 			   /// 
		SPHERE, 		   /// 
		CAPSULE,  		   /// 
		TRUNCATED_PYRAMID, /// 
		RAY,			   /// 
		MESH			   /// 
	};

	class TriangleGeometry;

	class GeometryPrimitiveBase
	{
	public:

		GeometryPrimitiveType type = GeometryPrimitiveType::NONE;

		virtual bool isTriangularMesh() = 0;
		virtual uint32_t getVertecesCount() = 0;
		virtual Vector3D getVertexPosition(uint32_t index) = 0;
		virtual uint32_t getTrianglesCount() = 0;
		virtual TriangleGeometry getTrianglePositions(uint32_t index) = 0;
		virtual bool isIdealGeometry() = 0;
	};
}

#endif