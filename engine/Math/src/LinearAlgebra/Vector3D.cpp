#include "Vector3D.h"

#include <cmath>

namespace mathem
{
	Vector3D& Vector3D::operator-()
	{
		i = -i;
		j = -j;
		k = -k;
		return *this;
	}

	Vector3D& Vector3D::operator*(float multiplier)
	{
		i *= multiplier;
		j *= multiplier;
		k *= multiplier;
		return *this;
	}

	Vector3D& Vector3D::operator/(float divider)
	{
		float multiplier = 1.0f / divider;
		i *= multiplier;
		j *= multiplier;
		k *= multiplier;
		return *this;
	}

	float Vector3D::getLength()
	{
		return (float)sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2));
	}

	void Vector3D::normalize()
	{
		float reciprocalLength = 1.0f / getLength();
		i *= reciprocalLength;
		j *= reciprocalLength;
		k *= reciprocalLength;
	}

	Vector3D operator+(Vector3D& vectorA, Vector3D& vectorB)
	{
		return { 
			vectorA.i + vectorB.i, 
			vectorA.j + vectorB.j, 
			vectorA.k + vectorB.k };
	}

	Vector3D operator-(Vector3D& vectorA, Vector3D& vectorB)
	{
		return {
			vectorA.i - vectorB.i,
			vectorA.j - vectorB.j,
			vectorA.k - vectorB.k };
	}

	float dotProduct(Vector3D vectorA, Vector3D vectorB)
	{
		return vectorA.i * vectorB.i + vectorA.j * vectorB.j + vectorA.k * vectorB.k;
	}

	Vector3D crossProduct(Vector3D vectorA, Vector3D vectorB)
	{
		return { vectorA.j * vectorB.k - vectorB.j * vectorA.k,
				 vectorB.i * vectorA.k - vectorA.i * vectorB.k,
				 vectorA.i * vectorB.j - vectorB.i * vectorA.j };
	}
}