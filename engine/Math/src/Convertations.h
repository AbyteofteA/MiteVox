
#ifndef CONVERTATIONS_H
#define CONVERTATIONS_H

#include <cmath>
#include "engine/Math/src/Matrix.h"
#include "engine/Math/src/Vector.h"
#include "engine/Math/src/Quaternion.h"
#include "engine/Math/src/Geometry/GeometryTransform.h"

namespace mathem
{
	float toRadians(float degrees);
	float toDegrees(float radians);

	Quaternion matrixToQuaternion(Matrix4x4& matrix);
	GeometryTransform matrixToTransform(Matrix4x4 matrix);

	Matrix4x4 quaternionToMatrix(Quaternion& quaternion);
	Matrix4x4 transformToMatrix(GeometryTransform& transform);

	Matrix4x4 toMatrix4x4(Matrix3x3 matrix);
	Matrix3x3 toMatrix3x3(Matrix4x4 matrix);
}

#endif