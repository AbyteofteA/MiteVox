
#ifndef MATH_INTERTOLATION_H
#define MATH_INTERTOLATION_H

#include "DataStructures/Buffer.h"

namespace mathem
{
	inline float interpolate(float x0, float x1, float a)
	{
		return x0 + (x1 - x0) * a;
	}

	inline void interpolate(float x0, float y0, float x1, float y1, Buffer<float>* buffer)
	{
		x0 = (float)floor(x0);
		x1 = (float)ceil(x1);
		if (x0 == x1)
		{
			buffer->resize(1);
			buffer->setSizeData(1);
			buffer->data[0] = y0;
			return;
		}

		float dx = x1 - x0;
		float dy = y1 - y0;

		float slope = 0;
		if (dx != 0)
			slope = dy / (float)dx;

		unsigned int iMax = (unsigned int)(x1 - x0);
		buffer->resize(iMax);
		buffer->setSizeData(iMax);

		for (unsigned int i = 0; i < buffer->getSizeData(); i++)
		{
			if (i == 0)
				buffer->data[i] = y0;
			else
				buffer->data[i] = buffer->data[i - 1] + slope;
		}
	}

}

#endif