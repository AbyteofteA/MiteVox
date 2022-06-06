
#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H

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
		inline Matrix(std::initializer_list<T> initializerList);

		T& at(size_t indexX, size_t indexY);
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
	T& MATRIX::at(size_t indexX, size_t indexY)
	{
		size_t actualIndex = indexX * DimensionY + indexY;
		// TODO: add bound check
		return data[actualIndex];
	}
}

#undef VECTOR_TEMPLATE
#undef VECTOR

#endif