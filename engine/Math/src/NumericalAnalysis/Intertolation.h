
#ifndef MATH_INTERTOLATION_H
#define MATH_INTERTOLATION_H

#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/Math/src/Quaternion.h"

namespace mathem
{
	template <typename T>
	inline T lerp(T x1, T x2, float slope)
	{
		T result = x2;
		result -= x1;
		result *= slope;
		result += x1;
		return result;
	}

	template <>
	inline Quaternion lerp<Quaternion>(Quaternion x1, Quaternion x2, float slope)
	{
		// TODO: implement slerp
		Quaternion result = x2 - x1;
		result *= slope;
		result += x1;
		result.normalize();
		return result;
	}

	inline float interpolate(float x0, float x1, float a)
	{
		return x0 + (x1 - x0) * a;
	}

	inline void interpolate(float x0, float y0, float x1, float y1, safety::SafeFloatArray* buffer)
	{
		x0 = (float)floor(x0);
		x1 = (float)ceil(x1);
		if (x0 == x1)
		{
			buffer->resize(1);
			buffer->setElement(0, y0);
			return;
		}

		float dx = x1 - x0;
		float dy = y1 - y0;

		float slope = 0;
		if (dx != 0)
			slope = dy / (float)dx;

		size_t bufferElementsCount = (unsigned int)(x1 - x0);
		buffer->resize(bufferElementsCount);
		for (unsigned int i = 0; i < bufferElementsCount; i++)
		{
			if (i == 0)
				buffer->setElement(i, y0);
			else
				buffer->setElement(i, buffer->getElement(i - 1) + slope);
		}
	}
}

#endif