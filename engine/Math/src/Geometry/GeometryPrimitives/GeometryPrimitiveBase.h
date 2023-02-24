#ifndef GEOMETRYPRIMITIVEBASE_H
#define GEOMETRYPRIMITIVEBASE_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"

#include <cstdint>

namespace mathem
{
	enum struct GeometryPrimitiveType : uint8_t 
	{ 
		NONE = 0, 			/// 
		POINT,				/// 
		LINE,				/// 
		AXIS_ALIGNED_BOX,	/// 
		BOX, 				/// 
		SPHERE, 			/// 
		CAPSULE,  			/// 
		TRUNCATED_PYRAMID,	/// 
		RAY,				/// 
		MESH, 				/// 
	};

	class TriangleGeometry3D;

	class GeometryPrimitiveBase
	{
	public:

		GeometryPrimitiveType getType();
		size_t getFaceVerteces(size_t vertexIndex, Vector3D normal, Vector3D* faceVerteces, GeometryTransform* geometryPrimitiveTransform, float equalityTolerance);

		virtual bool isTriangularMesh() = 0;
		virtual uint32_t getVertecesCount() = 0;
		virtual Vector3D getVertexPosition(uint32_t index) = 0;
		virtual uint32_t getTrianglesCount() = 0;
		virtual TriangleGeometry3D getTrianglePositions(uint32_t index) = 0;
		virtual bool isIdealGeometry() = 0;

	protected:

		GeometryPrimitiveType type = GeometryPrimitiveType::NONE;
	};
}

#endif