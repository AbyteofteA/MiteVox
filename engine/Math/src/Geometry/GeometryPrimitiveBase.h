
#ifndef GEOMETRYPRIMITIVEBASE_H
#define GEOMETRYPRIMITIVEBASE_H

#include "engine/Math/src/LinearAlgebra/Point3D.h"
#include "engine/Math/src/LinearAlgebra/Vector3D.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"

#include <cstdint>

namespace mathem
{
	enum struct GeometryPrimitiveType : uint8_t 
	{ 
		NONE = 0, 		   /// 
		TRIANGLE,		   /// 
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
		virtual uint32_t getPointsCount() = 0;
		virtual Vector3D getPoint(uint32_t index) = 0;
		virtual Vector3D getPoint(uint32_t index, GeometryTransform* globalTransform) = 0;
		virtual uint32_t getTrianglesCount() = 0;
		virtual TriangleGeometry getTriangle(uint32_t index) = 0;
		virtual TriangleGeometry getTriangle(uint32_t index, GeometryTransform* globalTransform) = 0;
		virtual bool isIdealGeometry() = 0;
	};
}

#endif