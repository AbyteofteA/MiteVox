#include "Convertations.h"

#include "engine/Math/src/MathConstants.h"

namespace mathem
{
	float toRadians(float degrees)
	{
		return degrees * (float)mathem::PI / 180.0f;
	}

	float toDegrees(float radians)
	{
		return radians * 180.0f / (float)mathem::PI;
	}

	Quaternion matrixToQuaternion(Matrix4x4& matrix)
	{
		Quaternion resultQuaternion;

		float trace = matrix.m00() + matrix.m11() + matrix.m22();
		if (trace > 0.0f)
		{
			float tmp = 0.5f / sqrt(1.0f + trace);
			resultQuaternion = Quaternion(
				tmp * (matrix.m12() - matrix.m21()), 
				tmp * (matrix.m20() - matrix.m02()), 
				tmp * (matrix.m01() - matrix.m10()), 
				0.25f / tmp);
		}
		else if ((matrix.m00() > matrix.m11()) && (matrix.m00() > matrix.m22()))
		{
			float tmp = 0.5f / sqrt(1.0f + matrix.m00() - matrix.m11() - matrix.m22());
			resultQuaternion = Quaternion(
				0.25f / tmp, 
				tmp * (matrix.m10() + matrix.m01()), 
				tmp * (matrix.m20() + matrix.m02()), 
				tmp * (matrix.m12() - matrix.m21()));
		}
		else if (matrix.m11() > matrix.m22())
		{
			float tmp = 0.5f / sqrt(1.0f + matrix.m11() - matrix.m00() - matrix.m22());
			resultQuaternion = Quaternion(
				tmp * (matrix.m10() + matrix.m01()), 
				0.25f / tmp, 
				tmp * (matrix.m21() + matrix.m12()), 
				tmp * (matrix.m20() - matrix.m02()));
		}
		else
		{
			float tmp = 0.5f / sqrt(1.0f + matrix.m22() - matrix.m00() - matrix.m11());
			resultQuaternion = Quaternion(
				tmp * (matrix.m20() + matrix.m02()), 
				tmp * (matrix.m21() + matrix.m12()), 
				0.25f / tmp, 
				tmp * (matrix.m01() - matrix.m10()));
		}
		resultQuaternion.normalize();
		return resultQuaternion;
	}

	GeometryTransform matrixToTransform(Matrix4x4 matrix)
	{
		GeometryTransform resultTransform;

		// Extract translation
		resultTransform.translation.x() = matrix.m30();
		resultTransform.translation.y() = matrix.m31();
		resultTransform.translation.z() = matrix.m32();
		matrix.m30() = 0.0f;
		matrix.m31() = 0.0f;
		matrix.m32() = 0.0f;

		// Extract scale
		Vector3D tmpVector;
		tmpVector = { matrix.m00(), matrix.m01(), matrix.m02() };
		resultTransform.scale.x() = tmpVector.getLength();
		tmpVector = { matrix.m10(), matrix.m11(), matrix.m12() };
		resultTransform.scale.y() = tmpVector.getLength();
		tmpVector = { matrix.m20(), matrix.m21(), matrix.m22() };
		resultTransform.scale.z() = tmpVector.getLength();
		float oneOverScaleX = 1.0f / resultTransform.scale.x();
		matrix.m00() *= oneOverScaleX;
		matrix.m01() *= oneOverScaleX;
		matrix.m02() *= oneOverScaleX;
		float oneOverScaleY = 1.0f / resultTransform.scale.y();
		matrix.m10() *= oneOverScaleY;
		matrix.m11() *= oneOverScaleY;
		matrix.m12() *= oneOverScaleY;
		float oneOverScaleZ = 1.0f / resultTransform.scale.z();
		matrix.m20() *= oneOverScaleZ;
		matrix.m21() *= oneOverScaleZ;
		matrix.m22() *= oneOverScaleZ;

		// Extract rotation
		resultTransform.rotation = matrixToQuaternion(matrix);

		return resultTransform;
	}

	Matrix4x4 quaternionToMatrix(Quaternion& quaternion)
	{
		Matrix4x4 resultMatrix4x4;
		resultMatrix4x4.makeIdentity();
		float quaternionX_Squared = std::pow(quaternion.components.x(), 2.0f);
		float quaternionY_Squared = std::pow(quaternion.components.y(), 2.0f);
		float quaternionZ_Squared = std::pow(quaternion.components.z(), 2.0f);
		float quaternionS_Squared = std::pow(quaternion.components.s(), 2.0f);
		float quaternion_XxY = quaternion.components.x() * quaternion.components.y();
		float quaternion_XxZ = quaternion.components.x() * quaternion.components.z();
		float quaternion_YxZ = quaternion.components.y() * quaternion.components.z();
		float quaternion_XxS = quaternion.components.x() * quaternion.components.s();
		float quaternion_YxS = quaternion.components.y() * quaternion.components.s();
		float quaternion_ZxS = quaternion.components.z() * quaternion.components.s();
		resultMatrix4x4.at(0, 0) = 2.0f * (quaternionS_Squared + quaternionX_Squared) - 1.0f;
		resultMatrix4x4.at(1, 0) = 2.0f * (quaternion_XxY - quaternion_ZxS);
		resultMatrix4x4.at(2, 0) = 2.0f * (quaternion_XxZ + quaternion_YxS);
		resultMatrix4x4.at(0, 1) = 2.0f * (quaternion_XxY + quaternion_ZxS);
		resultMatrix4x4.at(1, 1) = 2.0f * (quaternionS_Squared + quaternionY_Squared) - 1.0f;
		resultMatrix4x4.at(2, 1) = 2.0f * (quaternion_YxZ - quaternion_XxS);
		resultMatrix4x4.at(0, 2) = 2.0f * (quaternion_XxZ - quaternion_YxS);
		resultMatrix4x4.at(1, 2) = 2.0f * (quaternion_YxZ + quaternion_XxS);
		resultMatrix4x4.at(2, 2) = 2.0f * (quaternionS_Squared + quaternionZ_Squared) - 1.0f;
		return resultMatrix4x4;
	}

	Matrix4x4 transformToMatrix(GeometryTransform& transform)
	{
		// Encode rotation
		Matrix4x4 rotationMatrix4x4 = quaternionToMatrix(transform.rotation);

		// Encode scale
		Matrix4x4 scaleMatrix;
		scaleMatrix.makeIdentity();
		scaleMatrix.m00() = transform.scale.x();
		scaleMatrix.m11() = transform.scale.y();
		scaleMatrix.m22() = transform.scale.z();
		scaleMatrix.m33() = 1.0f;

		Matrix4x4 resultMatrix4x4;
		resultMatrix4x4 = multiply(scaleMatrix, rotationMatrix4x4);

		// Encode translation
		resultMatrix4x4.at(3, 0) += transform.translation.x();
		resultMatrix4x4.at(3, 1) += transform.translation.y();
		resultMatrix4x4.at(3, 2) += transform.translation.z();

		return resultMatrix4x4;
	}

	Matrix4x4 toMatrix4x4(Matrix3x3 matrix)
	{
		Matrix4x4 resultMatrix;
		resultMatrix.makeIdentity();
		resultMatrix.m00() = matrix.m00();
		resultMatrix.m01() = matrix.m01();
		resultMatrix.m02() = matrix.m02();
		resultMatrix.m10() = matrix.m10();
		resultMatrix.m11() = matrix.m11();
		resultMatrix.m12() = matrix.m12();
		resultMatrix.m20() = matrix.m20();
		resultMatrix.m21() = matrix.m21();
		resultMatrix.m22() = matrix.m22();
		return resultMatrix;
	}

	Matrix3x3 toMatrix3x3(Matrix4x4 matrix)
	{
		Matrix3x3 resultMatrix;
		resultMatrix.m00() = matrix.m00();
		resultMatrix.m01() = matrix.m01();
		resultMatrix.m02() = matrix.m02();
		resultMatrix.m10() = matrix.m10();
		resultMatrix.m11() = matrix.m11();
		resultMatrix.m12() = matrix.m12();
		resultMatrix.m20() = matrix.m20();
		resultMatrix.m21() = matrix.m21();
		resultMatrix.m22() = matrix.m22();
		return resultMatrix;
	}
}