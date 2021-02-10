
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
		template <typename T>
		friend class FilterLayer2DBase;

	public:

		T* kernels;

		/// <summary>
		/// Constructor for a convolutional filter.
		/// </summary>
		inline Filter2D(
			size_t _amountOfKernels,
			size_t _size,
			FillType _fillType = FillType::ZERO,
			PaddingType _paddingType = PaddingType::SAME,
			size_t _padding = 0,
			size_t _stride = 0);

		/// <summary>
		/// Constructor for a subsampling filter.
		/// </summary>
		inline Filter2D(
			size_t _size,
			FillType _fillType = FillType::ZERO,
			PaddingType _paddingType = PaddingType::SAME,
			size_t _padding = 0,
			size_t _stride = 0);

		~Filter2D();

		inline void setFilter();
		inline void setFilter(T value);

		// Getters //

		inline size_t getSize();
		inline size_t getStride();
		inline PaddingType getPaddingType();
		inline size_t getPadding();
		inline FillType getFillType();

		inline size_t getAmountOfKernels();

	private:

		size_t size;
		size_t stride;
		PaddingType paddingType;
		size_t padding;
		FillType fillType;

		size_t kernelsSizeInBytes;
		size_t amountOfKernels;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline Filter2D<T>::Filter2D(
		size_t _amountOfKernels,
		size_t _size,
		FillType _fillType,
		PaddingType _paddingType,
		size_t _padding,
		size_t _stride)
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

	template <typename T>
	inline Filter2D<T>::Filter2D(
		size_t _size,
		FillType _fillType,
		PaddingType _paddingType,
		size_t _padding,
		size_t _stride)
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

	template <typename T>
	inline Filter2D<T>::~Filter2D()
	{
		free(kernels);
	}

	template <typename T>
	inline void Filter2D<T>::setFilter()
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

	template <typename T>
	inline void Filter2D<T>::setFilter(T value)
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

	template <typename T>
	inline size_t Filter2D<T>::getSize()
	{
		return size;
	}

	template <typename T>
	inline size_t Filter2D<T>::getStride()
	{
		return stride;
	}

	template <typename T>
	inline PaddingType Filter2D<T>::getPaddingType()
	{
		return paddingType;
	}

	template <typename T>
	inline size_t Filter2D<T>::getPadding()
	{
		return padding;
	}

	template <typename T>
	inline FillType Filter2D<T>::getFillType()
	{
		return fillType;
	}

	template <typename T>
	inline size_t Filter2D<T>::getAmountOfKernels()
	{
		return amountOfKernels;
	}

}

#endif