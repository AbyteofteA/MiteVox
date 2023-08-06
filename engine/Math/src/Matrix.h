#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Vector.h"

#include <cmath>
#include <cassert>
#include <initializer_list>
#include <algorithm>

// Following defines work only this file (are undefined at the end).
#define MATRIX_TEMPLATE template <typename T, size_t DimensionX, size_t DimensionY>
#define MATRIX Matrix<T, DimensionX, DimensionY>

namespace mathem
{
	MATRIX_TEMPLATE
	class Matrix
	{
	public:

		T data[DimensionX * DimensionY] = { 0 };

		explicit inline Matrix();
		explicit inline Matrix(safety::SafeFloatArray* columnMajorArray4x4);
		inline Matrix(std::initializer_list<T> initializerList);

		inline T& at(size_t indexX, size_t indexY);

		inline T& m00();
		inline T& m01();
		inline T& m02();
		inline T& m03();
		
		inline T& m10();
		inline T& m11();
		inline T& m12();
		inline T& m13();
		
		inline T& m20();
		inline T& m21();
		inline T& m22();
		inline T& m23();
		
		inline T& m30();
		inline T& m31();
		inline T& m32();
		inline T& m33();

		inline void makeZero();
		inline void makeIdentity();
		inline void transpose();
	};

	typedef Matrix<float, 2, 2> Matrix2x2;
	typedef Matrix<float, 3, 3> Matrix3x3;
	typedef Matrix<float, 4, 4> Matrix4x4;

	inline float computeDeterminant(Matrix3x3 matrix);
	inline Matrix3x3 getInverse(Matrix3x3 matrix);

	inline Vector3D multiply(Matrix3x3 matrix, Vector3D vector);

#define SQUARE_MATRIX Matrix<float, Dimension, Dimension>
	template <size_t Dimension>
	inline SQUARE_MATRIX multiply(SQUARE_MATRIX& matrix1, SQUARE_MATRIX& matrix2);


	// IMPLEMENTATION BELOW //


	MATRIX_TEMPLATE
	inline MATRIX::Matrix()
	{
		assert((DimensionX > 0) && "ERROR: the mathem::Matrix cannot be that small!");
		assert((DimensionY > 0) && "ERROR: the mathem::Matrix cannot be that small!");

		makeZero();
	}

	MATRIX_TEMPLATE
	inline MATRIX::Matrix(safety::SafeFloatArray* columnMajorArray4x4)
	{
		size_t valuesCountToInitialize = columnMajorArray4x4->getElementsCount();

		size_t matrixElementsCount = DimensionX * DimensionY;
		if (valuesCountToInitialize < matrixElementsCount)
		{
			valuesCountToInitialize = matrixElementsCount;
		}
		for (size_t index = 0; index < valuesCountToInitialize; ++index)
		{
			data[index] = columnMajorArray4x4->getElement(index);
		}
	}

	MATRIX_TEMPLATE
	inline MATRIX::Matrix(std::initializer_list<T> initializerList)
	{
		assert((DimensionX > 0) && "ERROR: the mathem::Matrix cannot be that small!");
		assert((DimensionY > 0) && "ERROR: the mathem::Matrix cannot be that small!");
		assert((initializerList.size() <= DimensionX * DimensionY) && "ERROR: incorrect initializer list for the mathem::Matrix!");

		makeZero();
		
		for (size_t index = 0; index < (DimensionX * DimensionY); ++index)
		{
			data[index] = *(initializerList.begin() + index);
		}
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::at(size_t indexX, size_t indexY)
	{
		size_t actualIndex = indexX * DimensionY + indexY;
		// TODO: add bound check
		return data[actualIndex];
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m00()
	{
		return at(0, 0);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m01()
	{
		return at(0, 1);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m02()
	{
		return at(0, 2);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m03()
	{
		return at(0, 3);
	}
	
	MATRIX_TEMPLATE
	inline T& MATRIX::m10()
	{
		return at(1, 0);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m11()
	{
		return at(1, 1);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m12()
	{
		return at(1, 2);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m13()
	{
		return at(1, 3);
	}
	
	MATRIX_TEMPLATE
	inline T& MATRIX::m20()
	{
		return at(2, 0);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m21()
	{
		return at(2, 1);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m22()
	{
		return at(2, 2);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m23()
	{
		return at(2, 3);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m30()
	{
		return at(3, 0);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m31()
	{
		return at(3, 1);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m32()
	{
		return at(3, 2);
	}

	MATRIX_TEMPLATE
	inline T& MATRIX::m33()
	{
		return at(3, 3);
	}

	MATRIX_TEMPLATE
	inline void MATRIX::makeIdentity()
	{
		makeZero();

		size_t minDimension = DimensionX;
		if (DimensionX > DimensionY)
		{
			minDimension = DimensionY;
		}

		for (size_t index = 0; index < minDimension; ++index)
		{
			at(index, index) = 1.0f;
		}
	}

	MATRIX_TEMPLATE
	inline void MATRIX::makeZero()
	{
		memset(data, 0, DimensionX * DimensionY * sizeof(T));
	}

	MATRIX_TEMPLATE
	inline void MATRIX::transpose()
	{
		for (size_t row = 0; row < DimensionX; ++row)
		{
			for (size_t column = 0; column < DimensionY; ++column)
			{
				if (row == column)
				{
					continue;
				}
				std::swap(at(row, column), at(column, row));
			}
		}
	}

	inline float computeDeterminant(Matrix3x3 matrix)
	{
		float term1 = matrix.m00() * (matrix.m11() * matrix.m22() - matrix.m12() * matrix.m21());
		float term2 = matrix.m01() * (matrix.m10() * matrix.m22() - matrix.m12() * matrix.m20());
		float term3 = matrix.m02() * (matrix.m10() * matrix.m21() - matrix.m11() * matrix.m20());
		return term1 - term2 + term3;
	}

	inline Matrix3x3 getInverse(Matrix3x3 matrix)
	{
		float inverseDeterminant = 1.0f / computeDeterminant(matrix);
		
		float subDeterminant00 = matrix.m11() * matrix.m22() - matrix.m21() * matrix.m12();
		float subDeterminant01 = matrix.m02() * matrix.m21() - matrix.m01() * matrix.m22();
		float subDeterminant02 = matrix.m01() * matrix.m12() - matrix.m02() * matrix.m11();
		float subDeterminant10 = matrix.m12() * matrix.m20() - matrix.m10() * matrix.m22();
		float subDeterminant11 = matrix.m00() * matrix.m22() - matrix.m02() * matrix.m20();
		float subDeterminant12 = matrix.m10() * matrix.m02() - matrix.m00() * matrix.m12();
		float subDeterminant20 = matrix.m10() * matrix.m21() - matrix.m20() * matrix.m11();
		float subDeterminant21 = matrix.m20() * matrix.m01() - matrix.m00() * matrix.m21();
		float subDeterminant22 = matrix.m00() * matrix.m11() - matrix.m10() * matrix.m01();

		matrix.m00() = inverseDeterminant * subDeterminant00;
		matrix.m01() = inverseDeterminant * subDeterminant01;
		matrix.m02() = inverseDeterminant * subDeterminant02;
		matrix.m10() = inverseDeterminant * subDeterminant10;
		matrix.m11() = inverseDeterminant * subDeterminant11;
		matrix.m12() = inverseDeterminant * subDeterminant12;
		matrix.m20() = inverseDeterminant * subDeterminant20;
		matrix.m21() = inverseDeterminant * subDeterminant21;
		matrix.m22() = inverseDeterminant * subDeterminant22;

		return matrix;
	}

	inline Vector3D multiply(Matrix3x3 matrix, Vector3D vector)
	{
		Vector3D resultVector;
		resultVector.x() = matrix.m00() * vector.x() + matrix.m01() * vector.y() + matrix.m02() * vector.z();
		resultVector.y() = matrix.m10() * vector.x() + matrix.m11() * vector.y() + matrix.m12() * vector.z();
		resultVector.z() = matrix.m20() * vector.x() + matrix.m21() * vector.y() + matrix.m22() * vector.z();
		return resultVector;
	}

	template <size_t Dimension>
	inline SQUARE_MATRIX multiply(SQUARE_MATRIX& matrix1, SQUARE_MATRIX& matrix2)
	{
		SQUARE_MATRIX resultMatrix;
		resultMatrix.makeIdentity();
		for (size_t row = 0; row < Dimension; ++row)
		{
			for (size_t column = 0; column < Dimension; ++column)
			{
				float result = 0.0f;
				for (size_t i = 0; i < Dimension; ++i)
				{
					result += matrix1.at(row, i) * matrix2.at(i, column);
				}
				resultMatrix.at(row, column) = result;
			}
		}
		return resultMatrix;
	}
}

#undef VECTOR_TEMPLATE
#undef VECTOR

#endif