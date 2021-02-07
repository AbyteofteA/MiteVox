
#ifndef FILTER_2D_H
#define FILTER_2D_H

#include <string>

namespace aimods
{
	enum struct PaddingType { NO_PADDING = 0, SAME, ARBITRARY, DILATED};
	enum struct FillType { ZERO = 0, MAX, BORDER, REPEAT, REPEAT_MIRRORED};

	template <typename T>
	class Filter2D
	{
	public:

		size_t size;
		size_t stride;
		PaddingType paddingType;
		size_t padding;
		FillType fillType;

		T* kernels;
		size_t kernelsSizeInBytes;
		size_t amountOfKernels;

		/// <summary>
		/// Constructor for a convolutional filter.
		/// </summary>
		Filter2D(
			size_t _amountOfKernels,
			size_t _size,
			FillType _fillType = FillType::ZERO,
			PaddingType _paddingType = PaddingType::SAME,
			size_t _padding = 0,
			size_t _stride = 0)
		{
			// Kernel size must be odd.
			if (_size % 2 == 0)
			{
				size = _size + 1;
			}
			else
			{
				size = _size;
			}

			stride = _stride;
			paddingType = _paddingType;
			padding = _padding;
			fillType = _fillType;

			amountOfKernels = _amountOfKernels;
			kernelsSizeInBytes = amountOfKernels * size * size * sizeof(T);
			kernels = (T*)realloc(kernels, kernelsSizeInBytes);
			memset(kernels, 0, kernelsSizeInBytes);
		}
		/// <summary>
		/// Constructor for a subsampling filter.
		/// </summary>
		Filter2D(
			size_t _size,
			FillType _fillType = FillType::ZERO,
			PaddingType _paddingType = PaddingType::SAME,
			size_t _padding = 0,
			size_t _stride = 0)
		{
			// Kernel size must be odd.
			if (_size % 2 == 0)
			{
				size = _size + 1;
			}
			else
			{
				size = _size;
			}

			stride = _stride;
			paddingType = _paddingType;
			padding = _padding;
			fillType = _fillType;

			kernels = nullptr;
			kernelsSizeInBytes = 0;
			amountOfKernels = 1;
		}
		~Filter2D()
		{
			free(kernels);
		}

		void setFilter(T value)
		{
			size_t kernelOffset = size * size;
			for (size_t j = 0; j < size; j++)
			{
				for (size_t i = 0; i < size; i++)
				{
					for (size_t k = 0; k < amountOfKernels; k++)
					{
						kernels[i + j * size + k * kernelOffset] = value;
					}
				}
			}
		}
		void setFilter()
		{
			size_t kernelOffset = size * size;
			for (size_t j = 0; j < size; j++)
			{
				for (size_t i = 0; i < size; i++)
				{
					for (size_t k = 0; k < amountOfKernels; k++)
					{
						kernels[i + j * size + k * kernelOffset] = 
							(float)(rand() % 200 - 100) / 50;
					}
				}
			}
		}
	};

}

#endif