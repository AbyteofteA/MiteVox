#ifndef GEOMETRYTRANSFORM_H
#define GEOMETRYTRANSFORM_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Quaternion.h"
#include "engine/Math/src/Matrix.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry3D.h"
#include "engine/CodeSafety/src/SafeArray.h"

namespace mathem
{
	class GeometryTransform
	{
	public:

		Vector3D translation = { 0.0f, 0.0f, 0.0f };
		Quaternion rotation;
		Vector3D scale = { 1.0f, 1.0f, 1.0f };

		void reset();

		GeometryTransform getInverse();

		void applyTo(Vector3D& vector);
		Vector3D applyToCopy(Vector3D vector);
		void applyInverseTo(Vector3D& vector);
		Vector3D applyInverseToCopy(Vector3D vector);
		void applyTo(TriangleGeometry3D& triangle);
		TriangleGeometry3D applyToCopy(TriangleGeometry3D triangle);

		void operator*=(GeometryTransform& otherTransform);
		GeometryTransform operator*(GeometryTransform& otherTransform);
	};
}

#endif
