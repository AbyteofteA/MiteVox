
#ifndef GEOMETRYTRANSFORM_H
#define GEOMETRYTRANSFORM_H

#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Matrix.h"
#include "engine/Math/src/Geometry/GeometryPrimitives/TriangleGeometry.h"
#include "engine/CodeSafety/src/SafeArray.h"

namespace mathem
{
	class GeometryTransform
	{
	public:

		Vector3D translation = { 0.0f, 0.0f, 0.0f };
		Vector4D rotation = { 0.0f, 0.0f, 0.0f, 1.0f }; /// TODO: move to quaternions
		Vector3D scale = { 1.0f, 1.0f, 1.0f };

		GeometryTransform();
		GeometryTransform(safety::SafeFloatArray* columnMajorArray4x4);
		// TODO: GeometryTransform(Matrix4x4* transformationMatrix);
		// TODO: GeometryTransform(Quaternion);

		void reset();

		void fromArray4x4(safety::SafeFloatArray* columnMajorMatrix4x4);
		// TODO: fromMatrix4x4(Matrix4x4* transformationMatrix);
		void fromScaleArray(safety::SafeFloatArray* scaleArray);
		void fromRotationArray(safety::SafeFloatArray* rotationArray);
		void fromTranslationArray(safety::SafeFloatArray* translationArray);
		// TODO: fromQuaternion(Quaternion);

		void applyTo(Vector3D& vector);
		Vector3D applyToCopy(Vector3D vector);
		void applyTo(TriangleGeometry& triangle);
		TriangleGeometry applyToCopy(TriangleGeometry triangle);
	};
}

#endif
