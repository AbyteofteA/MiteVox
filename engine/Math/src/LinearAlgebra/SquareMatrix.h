
#ifndef SQUAREMATRIX_H
#define SQUAREMATRIX_H

namespace mathem
{
	template <typename T, size_t Dim>
	class SquareMatrix
	{
	public:

		/// <summary>
		/// Copy assignment.
		/// </summary>
		SquareMatrix<T, Dim>& operator=(const SquareMatrix<T, Dim>& other);

		SquareMatrix<T, Dim>& operator+=(const SquareMatrix<T, Dim>& rhs);
		SquareMatrix<T, Dim>& operator-=(const SquareMatrix<T, Dim>& rhs);

		SquareMatrix<T, Dim>& operator*=(T value);
		//Vector<T, Dim>& operator*=(const Vector<T, Dim>& rhs);
		SquareMatrix<T, Dim>& operator*=(const SquareMatrix<T, Dim>& rhs);
		
	private:

		const size_t size = Dim * Dim;
		T data[Dim * Dim] = { 0 };
	};


	// IMPLEMENTATION BELOW //


	template <typename T, size_t Dim>
	SquareMatrix<T, Dim>& SquareMatrix<T, Dim>::operator=(const SquareMatrix<T, Dim>& other)
	{
		// Guard self assignment
		if (this == &other)
		{
			return *this;
		}

		for (size_t i = 0; i < size; i++)
		{
			this->data[i] = other.data[i];
		}

		return *this;
	}

	template <typename T, size_t Dim>
	SquareMatrix<T, Dim>& SquareMatrix<T, Dim>::operator+=(const SquareMatrix<T, Dim>& rhs)
	{
		for (size_t i = 0; i < size; i++)
		{
			this->data[i] += rhs.data[i];
		}
		return *this;
	}

	template <typename T, size_t Dim>
	SquareMatrix<T, Dim>& SquareMatrix<T, Dim>::operator-=(const SquareMatrix<T, Dim>& rhs)
	{
		for (size_t i = 0; i < size; i++)
		{
			this->data[i] -= rhs.data[i];
		}
		return *this;
	}

	template <typename T, size_t Dim>
	SquareMatrix<T, Dim>& SquareMatrix<T, Dim>::operator*=(T value)
	{
		for (size_t i = 0; i < size; i++)
		{
			this->data[i] *= value;
		}
		return *this;
	}

	// TODO: Vector<T, Dim>& SquareMatrix<T, Dim>::operator*=(const Vector<T, Dim>& rhs);

	// TODO:
	template <typename T, size_t Dim>
	SquareMatrix<T, Dim>& SquareMatrix<T, Dim>::operator*=(const SquareMatrix<T, Dim>& rhs)
	{
		for (size_t i = 0; i < Dim; i++)
		{
			for (size_t i = 0; i < Dim; i++)
			{
				
			}
		}
		return *this;
	}

	template <typename T, size_t Dim>
	SquareMatrix<T, Dim> operator+(SquareMatrix<T, Dim> lhs, SquareMatrix<T, Dim> rhs)
	{
		lhs += rhs;
		return lhs;
	}

	template <typename T, size_t Dim>
	SquareMatrix<T, Dim> operator-(SquareMatrix<T, Dim> lhs, SquareMatrix<T, Dim> rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	template <typename T, size_t Dim>
	SquareMatrix<T, Dim> operator*(SquareMatrix<T, Dim> lhs, T value)
	{
		lhs *= value;
		return lhs;
	}

	// TODO: SquareMatrix<T, Dim> operator*(SquareMatrix<T, Dim> lhs, Vector<T, Dim> rhs);

	template <typename T, size_t Dim>
	SquareMatrix<T, Dim> operator*(SquareMatrix<T, Dim> lhs, SquareMatrix<T, Dim> rhs)
	{
		lhs *= rhs;
		return lhs;
	}
}

#endif