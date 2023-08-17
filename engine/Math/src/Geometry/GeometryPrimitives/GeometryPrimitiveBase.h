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
		RAY,				/// 
		PLANE,				/// 
		AXIS_ALIGNED_BOX,	/// 
		BOX, 				/// 
		SPHERE, 			/// 
		CAPSULE,  			/// 
		TRUNCATED_PYRAMID,	/// 
		MESH, 				/// 
	};

	struct Point
	{

	};

	struct Line
	{
		Vector3D point1 = { 0.0f, 0.0f, 0.0f };
		Vector3D point2 = { 0.0f, 1.0f, 0.0f };
	};

	class Ray
	{
		Vector3D position = { 0.0f, 0.0f, 0.0f };
		Vector3D direction = { 0.0f, 0.0f, -1.0f };
	};

	class Plane
	{
		Vector3D normal = { 0.0f, 1.0f, 0.0f };
		float position = 0.0f;
	};

	struct AxisAlignedBox
	{
		Vector3D position = { 0.0f, 0.0f, 0.0f };
		Vector3D halfSize = { 0.5f, 0.5f, 0.5f };
	};

	struct Box
	{
		GeometryTransform transform;
		Vector3D halfSize = { 0.5f, 0.5f, 0.5f };
	};

	struct Sphere
	{
		GeometryTransform transform;
		float radius = 0.5f;
	};

	struct Capsule
	{
		GeometryTransform transform;
		float halfHeight = 0.5f;
		float radius = 0.5f;
	};

	struct TruncatedPyramid
	{
		GeometryTransform transform;
		float FOV = 45.f;
		float halfWidth, halfHeight;
		float nearPlane = 0.1f;
		float farPlane = 100000.f;
	};

	union AnyGeometryPrimitive
	{
		Point point;
		Line line;
		Ray ray;
		Plane plane;
		AxisAlignedBox AABB;
		Box box;
		Sphere sphere;
		Capsule capsule;
		TruncatedPyramid truncatedPyramid;

		AnyGeometryPrimitive();
	};

	class TriangleGeometry3D;

	class GeometryPrimitiveBase
	{
	public:

		GeometryPrimitiveType getType();
		bool isTriangularMesh();

		size_t getFaceVerteces(size_t vertexIndex, Vector3D normal, Vector3D* faceVerteces, GeometryTransform* geometryPrimitiveTransform, float equalityTolerance);

		virtual uint32_t getVertecesCount() = 0;
		virtual Vector3D getVertexPosition(uint32_t index) = 0;
		virtual uint32_t getTrianglesCount() = 0;
		virtual TriangleGeometry3D getTrianglePositions(uint32_t index) = 0;

	protected:

		GeometryPrimitiveType type = GeometryPrimitiveType::NONE;
		AnyGeometryPrimitive anyPrimitive;
	};
}

#endif