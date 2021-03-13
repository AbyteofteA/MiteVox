
#ifndef MATH_VECTOR3D_H
#define MATH_VECTOR3D_H

namespace mathem
{
	template <typename T, size_t Dim>
	class Vector
	{
	public:

		T data[Dim] = { 0 };

		float getLength();
		void normalize();

		Vector<T, Dim>& operator=(T values[Dim])
		{
			for (size_t i = 0; i < Dim; i++)
			{
				this->data[i] = values[i];
			}
		}

		T operator[](size_t idx);
		const T& operator[](size_t idx) const;

	};

	typedef Vector<float, 2> Vector2D;
	typedef Vector<float, 3> Vector3D;


	// IMPLEMENTATION BELOW //


	template <typename T, size_t Dim>
	float Vector<T, Dim>::getLength()
	{
		T tmp = 0;
		for (size_t i = 0; i < Dim; i++)
		{
			tmp += pow(this->data[i], 2);
		}

		return (T)sqrt(tmp);
	}

	template <typename T, size_t Dim>
	void Vector<T, Dim>::normalize()
	{
		T length = getLength();

		for (size_t i = 0; i < Dim; i++)
		{
			this->data[i] /= length;
		}
	}

	template <typename T, size_t Dim>
	T Vector<T, Dim>::operator[](size_t idx)
	{
		return data[idx];
	}

	template <typename T, size_t Dim>
	const T& Vector<T, Dim>::operator[](size_t idx) const
	{
		return data[idx];
	}
}

#endif