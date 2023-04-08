#ifndef FILTER_2D_H
#define FILTER_2D_H

#include "engine/AIModels/src/Structure/Functions.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>
#include <algorithm>

namespace aimods
{
	/// <summary>
	/// Defines the region inside and outside of the map the filter can sample from.
	/// </summary>
	enum struct PaddingType 
	{ 
		NO_PADDING = 0,	/// The filter can sample from the map, but not from outside;
		SAME,			/// The padding is calculated so that input map is of the same size as output map;
		ARBITRARY,		/// The padding is set by user (via Filter2D::padding);
		DILATED			/// TODO: add description for PaddingType::DILATED.
	};

	/// <summary>
	/// Defines how elements outside of the map (image in case of 2D) are sampled.
	/// </summary>
	enum struct FillType 
	{ 
		ZERO,						/// Fill elements outside of the map with zeros;
		MAX,						/// Fill elements outside of the map with maximum values for the type;
		BORDER,						/// Fill elements outside of the map with nearest boarder pixels;
		REPEAT,						/// Tile elements outside of the map;
		REPEAT_HORIZONTALLY_ZERO,	/// Tile elements outside of the map horizontally, other values 
									///   outside of the map are filled with zeros;
		REPEAT_HORIZONTALLY_MAX,	/// Tile elements outside of the map horizontally, other values 
									///   outside of the map are filled with maximum values for the type;
		REPEAT_VERTICALLY_ZERO,		/// Tile elements outside of the map vertically, other values 
									///   outside of the map are filled with zeros;
		REPEAT_VERTICALLY_MAX,		/// Tile elements outside of the map vertically, other values 
									///   outside of the map are filled with maximum values for the type;
	};

	/// <summary>
	/// Filter2D is set of square kernels used for image processing.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class Filter2D
	{
		friend class Filter2DCodec;

		template <typename T>
		friend class FilterLayer2DBase;

	public:

		/// <summary>
		/// Constructor for a convolutional filter.
		/// </summary>
		inline Filter2D(
			size_t amountOfKernels,
			size_t dimension,
			size_t stride,
			FillType fillType = FillType::ZERO,
			PaddingType paddingType = PaddingType::SAME,
			size_t padding = 0);

		/// <summary>
		/// Constructor for a subsampling filter.
		/// </summary>
		inline Filter2D(
			size_t dimension,
			size_t stride,
			FillType fillType = FillType::ZERO,
			PaddingType paddingType = PaddingType::SAME,
			size_t padding = 0);

		~Filter2D();

		inline void setFilter(T value);
		inline void setFilterRandom();
		inline void setFilterIdentity();
		inline void setFilter();
		inline size_t computeFilterDataElementCount();
		inline size_t computeFilterDataSize();
		inline size_t computeKernelSize();
		inline size_t computeHalfDimension();

		inline size_t getDimension();
		inline size_t getStride();
		inline PaddingType getPaddingType();
		inline size_t getPadding();
		inline FillType getFillType();
		inline size_t getAmountOfKernels();
		inline size_t getKernelElementCount();
		inline T* getFilterData();

	private:

		size_t dimension;
		size_t stride;
		PaddingType paddingType;
		size_t padding;
		FillType fillType;
		size_t amountOfKernels;
		size_t kernelElementCount;

		safety::SafeArray<T> filterData;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline Filter2D<T>::Filter2D(
		size_t amountOfKernels,
		size_t dimension,
		size_t stride,
		FillType fillType,
		PaddingType paddingType,
		size_t padding)
	{
		this->amountOfKernels = std::max<size_t>(amountOfKernels, 1);

		// Kernel's dimension must be at least 2.
		this->dimension = std::max<size_t>(dimension, 2);

		this->fillType = fillType;
		this->paddingType = paddingType;
		this->padding = padding;
		this->kernelElementCount = this->dimension * this->dimension;

		// stride must be at least 1 to prevent the filter from getting stuck in place.
		this->stride = std::max<size_t>(stride, 1);

		size_t kernelsElementCount = computeFilterDataElementCount();
		this->filterData.resize(kernelsElementCount);
		this->filterData.setAllElements(0.0);
	}

	template <typename T>
	inline Filter2D<T>::Filter2D(
		size_t dimension,
		size_t stride,
		FillType fillType,
		PaddingType paddingType,
		size_t padding)
	{
		this->amountOfKernels = 1;

		// Kernel's dimension must be odd and at least 3.
		this->dimension = std::max<size_t>(dimension, 2);

		this->fillType = fillType;
		this->paddingType = paddingType;
		this->padding = padding;
		this->kernelElementCount = this->dimension * this->dimension;

		// stride must be at least 1 to prevent the filter from getting stuck in place.
		this->stride = std::max<size_t>(stride, 1);
	}

	template <typename T>
	inline Filter2D<T>::~Filter2D()
	{
		filterData.deallocate();
	}

	template <typename T>
	inline void Filter2D<T>::setFilter(T value)
	{
		if (filterData.getElementsCount() == 0)
		{
			return;
		}

		size_t kernelOffset = dimension * dimension;
		for (size_t kernelIndex = 0; kernelIndex < amountOfKernels; kernelIndex++)
		{
			for (size_t j = 0; j < dimension; j++)
			{
				for (size_t i = 0; i < dimension; i++)
				{
					filterData[kernelIndex * kernelOffset + i + j * dimension] = value;
				}
			}
		}
	}

	template <typename T>
	inline void Filter2D<T>::setFilterRandom()
	{
		size_t kernelOffset = dimension * dimension;
		for (size_t kernelIndex = 0; kernelIndex < amountOfKernels; kernelIndex++)
		{
			for (size_t j = 0; j < dimension; j++)
			{
				for (size_t i = 0; i < dimension; i++)
				{
					filterData[kernelIndex * kernelOffset + i + j * dimension] =
						(T)rand() / ((T)RAND_MAX * 10.0f);
				}
			}
		}
	}

	template <typename T>
	inline size_t Filter2D<T>::computeFilterDataElementCount()
	{
		return amountOfKernels * dimension * dimension;
	}

	template <typename T>
	inline size_t Filter2D<T>::computeFilterDataSize()
	{
		return amountOfKernels * dimension * dimension * sizeof(T);
	}
	
	template <typename T>
	inline size_t Filter2D<T>::computeKernelSize()
	{
		return kernelElementCount * sizeof(T);
	}

	template <typename T>
	inline size_t Filter2D<T>::computeHalfDimension()
	{
		return dimension / 2;
	}

	template <typename T>
	inline size_t Filter2D<T>::getDimension()
	{
		return dimension;
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

	template <typename T>
	inline size_t Filter2D<T>::getKernelElementCount()
	{
		return kernelElementCount;
	}

	template <typename T>
	inline T* Filter2D<T>::getFilterData()
	{
		return filterData.getElementsArray();
	}
}

#endif