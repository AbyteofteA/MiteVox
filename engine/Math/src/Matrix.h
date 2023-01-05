
#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H

#include "engine/CodeSafety/src/SafeArray.h"

#include <cmath>
#include <cassert>
#include <initializer_list>

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

		inline void makeIdentity();
	};

	typedef Matrix<float, 2, 2> Matrix2x2;
	typedef Matrix<float, 3, 3> Matrix3x3;
	typedef Matrix<float, 4, 4> Matrix4x4;


	// IMPLEMENTATION BELOW //


	MATRIX_TEMPLATE
	inline MATRIX::Matrix()
	{
		assert((DimensionX > 0) && "ERROR: the mathem::Matrix cannot be that small!");
		assert((DimensionY > 0) && "ERROR: the mathem::Matrix cannot be that small!");
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
}

#undef VECTOR_TEMPLATE
#undef VECTOR

#endif