
#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

#include <cmath>
#include <cassert>
#include <initializer_list>

// Following defines work only this file (are undefined at the end).
#define VECTOR_TEMPLATE template <typename T, size_t Dimension>
#define VECTOR Vector<T, Dimension>

namespace mathem
{
	VECTOR_TEMPLATE
	class Vector
	{
	public:

		T data[Dimension] = { 0 };

		explicit inline Vector();
		inline Vector(std::initializer_list<T> initializerList);

		inline T& u();
		inline T& v();

		inline T& x();
		inline T& y();
		inline T& z();

		inline T& i();
		inline T& j();
		inline T& k();

		inline T& s();

		inline T getLength();
		inline void normalize();

		inline void operator=(T values[Dimension]);
		inline T& operator[](size_t index);

		inline VECTOR& operator-();

		inline VECTOR operator*(T multiplier);
		inline void operator*=(T multiplier);
		inline T operator*(VECTOR& otherVector);

		inline VECTOR operator/(T divider);
		inline void operator/=(T divider);

		inline VECTOR operator+(T value);
		inline void operator+=(T value);
		inline VECTOR operator+(VECTOR& otherVector);
		inline void operator+=(VECTOR& otherVector);

		inline VECTOR operator-(T value);
		inline void operator-=(T value);
		inline VECTOR operator-(VECTOR& otherVector);
		inline void operator-=(VECTOR& otherVector);
	};
	
	typedef Vector<float, 2> Vector2D;
	typedef Vector<float, 3> Vector3D;
	typedef Vector<float, 4> Vector4D;

	Vector3D crossProduct(Vector3D vectorA, Vector3D vectorB);


	// IMPLEMENTATION BELOW //


	VECTOR_TEMPLATE
	inline VECTOR::Vector()
	{
		assert((Dimension > 1) && "ERROR: the mathem::Vector cannot be that small!");
	}

	VECTOR_TEMPLATE
	inline VECTOR::Vector(std::initializer_list<T> initializerList)
	{
		assert((Dimension > 1) && "ERROR: the mathem::Vector cannot be that small!");
		assert((initializerList.size() <= Dimension) && "ERROR: incorrect initializer list for the mathem::Vector!");
		
		for (size_t index = 0; index < Dimension; ++index)
		{
			data[index] = *(initializerList.begin() + index);
		}
	}

	VECTOR_TEMPLATE
	inline T& VECTOR::u()
	{
		return data[0];
	}

	VECTOR_TEMPLATE
	inline T& VECTOR::v()
	{
		return data[1];
	}

	VECTOR_TEMPLATE
	inline T& VECTOR::x()
	{
		return data[0];
	}

	VECTOR_TEMPLATE
	inline T& VECTOR::y()
	{
		return data[1];
	}

	VECTOR_TEMPLATE
	inline T& VECTOR::z()
	{
		assert((Dimension > 2) && "ERROR: the mathem::Vector::z is unaccessible!");
		return data[2];
	}

	VECTOR_TEMPLATE
	inline T& VECTOR::i()
	{
		return data[0];
	}

	VECTOR_TEMPLATE
	inline T& VECTOR::j()
	{
		return data[1];
	}

	VECTOR_TEMPLATE
	inline T& VECTOR::k()
	{
		assert((Dimension > 2) && "ERROR: the mathem::Vector::k is unaccessible!");
		return data[2];
	}

	VECTOR_TEMPLATE
	inline T& VECTOR::s()
	{
		assert((Dimension > 3) && "ERROR: the mathem::Vector::s is unaccessible!");
		return data[3];
	}

	VECTOR_TEMPLATE
	inline T VECTOR::getLength()
	{
		T tmp = (T)0;
		for (size_t i = 0; i < Dimension; ++i)
		{
			tmp += std::pow(this->data[i], 2);
		}
		return (T)std::sqrt(tmp);
	}

	VECTOR_TEMPLATE
	inline void VECTOR::normalize()
	{
		T length = getLength();
		if (length == (T)0)
		{
			return;
		}
		T reciprocalLength = (T)1 / length;
		for (size_t i = 0; i < Dimension; ++i)
		{
			this->data[i] *= reciprocalLength;
		}
	}

	VECTOR_TEMPLATE
	inline void VECTOR::operator=(T values[Dimension])
	{
		for (size_t i = 0; i < Dimension; ++i)
		{
			this->data[i] = values[i];
		}
	}

	VECTOR_TEMPLATE
	inline T& VECTOR::operator[](size_t index)
	{
		assert((Dimension > index) && "ERROR: the mathem::Vector::data[index] is unaccessible!");
		return data[index];
	}

	VECTOR_TEMPLATE
	inline VECTOR& VECTOR::operator-()
	{
		for (size_t i = 0; i < Dimension; ++i)
		{
			this->data[i] = -this->data[i];
		}
		return *this;
	}

	VECTOR_TEMPLATE
	inline VECTOR VECTOR::operator*(T multiplier)
	{
		VECTOR resultVector;
		for (size_t i = 0; i < Dimension; ++i)
		{
			resultVector[i] = this->data[i] * multiplier;
		}
		return resultVector;
	}

	VECTOR_TEMPLATE
	inline void VECTOR::operator*=(T multiplier)
	{
		for (size_t i = 0; i < Dimension; ++i)
		{
			this->data[i] *= multiplier;
		}
	}

	VECTOR_TEMPLATE
	inline T VECTOR::operator*(VECTOR& otherVector)
	{
		T resultScalar = (T)0;
		for (size_t i = 0; i < Dimension; ++i)
		{
			resultScalar += this->data[i] * otherVector[i];
		}
		return resultScalar;
	}

	VECTOR_TEMPLATE
	inline VECTOR VECTOR::operator/(T divider)
	{
		VECTOR resultVector;
		if (divider == (T)0)
		{
			return resultVector;
		}
		T multiplier = (T)1 / divider;
		for (size_t i = 0; i < Dimension; ++i)
		{
			resultVector[i] = this->data[i] * multiplier;
		}
		return resultVector;
	}

	VECTOR_TEMPLATE
	inline void VECTOR::operator/=(T divider)
	{
		if (divider == (T)0)
		{
			return;
		}
		T multiplier = (T)1 / divider;
		for (size_t i = 0; i < Dimension; ++i)
		{
			this->data[i] *= multiplier;
		}
	}

	VECTOR_TEMPLATE
	inline VECTOR VECTOR::operator+(T value)
	{
		VECTOR resultVector;
		for (size_t i = 0; i < Dimension; ++i)
		{
			resultVector[i] = this->data[i] + value;
		}
		return resultVector;
	}

	VECTOR_TEMPLATE
	inline void VECTOR::operator+=(T value)
	{
		for (size_t i = 0; i < Dimension; ++i)
		{
			this->data[i] += value;
		}
	}

	VECTOR_TEMPLATE
	inline VECTOR VECTOR::operator+(VECTOR& otherVector)
	{
		VECTOR resultVector;
		for (size_t i = 0; i < Dimension; ++i)
		{
			resultVector[i] = this->data[i] + otherVector[i];
		}
		return resultVector;
	}

	VECTOR_TEMPLATE
	inline void VECTOR::operator+=(VECTOR& otherVector)
	{
		for (size_t i = 0; i < Dimension; ++i)
		{
			this->data[i] += otherVector[i];
		}
	}

	VECTOR_TEMPLATE
	inline VECTOR VECTOR::operator-(T value)
	{
		VECTOR resultVector;
		for (size_t i = 0; i < Dimension; ++i)
		{
			resultVector[i] = this->data[i] - value;
		}
		return resultVector;
	}

	VECTOR_TEMPLATE
	inline void VECTOR::operator-=(T value)
	{
		for (size_t i = 0; i < Dimension; ++i)
		{
			this->data[i] -= value;
		}
	}

	VECTOR_TEMPLATE
	inline VECTOR VECTOR::operator-(VECTOR& otherVector)
	{
		VECTOR resultVector;
		for (size_t i = 0; i < Dimension; ++i)
		{
			resultVector[i] = this->data[i] - otherVector[i];
		}
		return resultVector;
	}

	VECTOR_TEMPLATE
	inline void VECTOR::operator-=(VECTOR& otherVector)
	{
		for (size_t i = 0; i < Dimension; ++i)
		{
			this->data[i] -= otherVector[i];
		}
	}
}

#undef VECTOR_TEMPLATE
#undef VECTOR

#endif