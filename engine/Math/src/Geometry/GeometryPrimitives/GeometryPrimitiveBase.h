#ifndef GEOMETRYPRIMITIVEBASE_H
#define GEOMETRYPRIMITIVEBASE_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/PlaneGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/AxisAlignedBoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/BoxGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/CapsuleGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/SphereGeometry.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TruncatedPyramidGeometry.h"
#include "engine/MiteVox/src/Mesh/MeshPrimitive.h"

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
		CONVEX_HULL,		/// 

		count
	};

	struct Point
	{
		Vector3D position = { 0.0f, 0.0f, 0.0f };
	};

	struct Line
	{
		Vector3D points[2] = { {0.0f, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f } };
	};

	struct Ray
	{
		Vector3D position = { 0.0f, 0.0f, 0.0f };
		Vector3D direction = { 0.0f, 0.0f, -1.0f };
	};

	

	union AnyGeometryPrimitive
	{
		Point point;
		Line line;
		Ray ray;
		PlaneGeometry plane;
		AxisAlignedBoxGeometry AABB;
		BoxGeometry box;
		SphereGeometry sphere;
		CapsuleGeometry capsule;
		TruncatedPyramidGeometry truncatedPyramid;
		mitevox::MeshPrimitive* convexHull;

		AnyGeometryPrimitive();
	};

	class TriangleGeometry3D;

	class GeometryPrimitiveBase
	{
	public:

		AnyGeometryPrimitive anyPrimitive;

		GeometryPrimitiveBase axisAlignedBoxToBox();

		Point* getPoint();
		Line* getLine();
		Ray* getRay();
		PlaneGeometry* getPlane();
		AxisAlignedBoxGeometry* getAxisAlignedBox();
		BoxGeometry* getBox();
		SphereGeometry* getSphere();
		CapsuleGeometry* getCapsule();
		TruncatedPyramidGeometry* getTruncatedPyramid();
		mitevox::MeshPrimitive* getConvexHull();

		void setPoint(Vector3D position = { 0.0f, 0.0f, 0.0f });
		void setLine(Vector3D point1 = { 0.0f, 0.0f, 0.0f }, Vector3D point2 = { 0.0f, 1.0f, 0.0f });
		void setRay(Vector3D position = { 0.0f, 0.0f, 0.0f }, Vector3D direction = { 0.0f, 0.0f, -1.0f });
		void setPlane(float position = 0.0f, Vector3D normal = { 0.0f, 1.0f, 0.0f });
		void setAxisAlignedBox(Vector3D position = { 0.0f, 0.0f, 0.0f }, Vector3D halfSize = { 0.5f, 0.5f, 0.5f });
		void setBox();
		void setBox(Vector3D halfSize, Vector3D translation);
		void setBox(Vector3D halfSize, GeometryTransform transform);
		void setSphere();
		void setSphere(float radius, GeometryTransform transform);
		void setCapsule();
		void setCapsule(float halfHeight, float radius, GeometryTransform transform);
		void setTruncatedPyramid();
		void setTruncatedPyramid(float FOV, float halfWidth, float halfHeight, float nearPlane, float farPlane, GeometryTransform transform);
		void setConvexHull(mitevox::MeshPrimitive* convexHull);

		float getVolume();

		GeometryPrimitiveType getType();
		bool isTriangularMesh();

		size_t getFaceVerteces(size_t vertexIndex, Vector3D normal, Vector3D* faceVerteces, GeometryTransform* geometryPrimitiveTransform, float equalityTolerance);

		uint32_t getVertecesCount();
		Vector3D getVertexPosition(uint32_t index);
		uint32_t getTrianglesCount();
		TriangleGeometry3D getTrianglePositions(uint32_t index);

	protected:

		GeometryPrimitiveType type = GeometryPrimitiveType::NONE;
	};
}

#endif