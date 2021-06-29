
#ifndef FILTER_2D_H
#define FILTER_2D_H

#include <string>
#include "engine/CodeSafety/src/CodeSafety.h"
#include "engine/AIModels/src/Structure/Functions.h"

namespace aimods
{
	/// <summary>
	/// Defines the region inside and outside of the map the filter can sample from.
	/// </summary>
	enum struct PaddingType 
	{ 
		NO_PADDING = 0,	/// The filter can sample from the map, but not from outside;
		SAME,			/// The padding is calculated so that input map is of the same size as output map;
		ARBITRARY,		/// The padding is set by user (via Filter2D::_padding);
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
			ActivationFunction activationFunction,
			FillType fillType = FillType::ZERO,
			PaddingType paddingType = PaddingType::SAME,
			size_t padding = 0);

		/// <summary>
		/// Constructor for a subsampling filter.
		/// </summary>
		inline Filter2D(
			size_t dimension,
			size_t stride,
			ActivationFunction activationFunction,
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

		// Getters //

		inline size_t getDimension();
		inline size_t getStride();
		inline PaddingType getPaddingType();
		inline size_t getPadding();
		inline FillType getFillType();
		inline size_t getAmountOfKernels();
		inline size_t getKernelElementCount();
		inline ActivationFunction getActivationFunction();
		inline T* getFilterData();

	private:

		size_t _dimension;
		size_t _stride;
		PaddingType _paddingType;
		size_t _padding;
		FillType _fillType;
		size_t _amountOfKernels;
		size_t _kernelElementCount;
		ActivationFunction _activationFunction;

		T* _filterData;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline Filter2D<T>::Filter2D(
		size_t amountOfKernels,
		size_t dimension,
		size_t stride,
		ActivationFunction activationFunction,
		FillType fillType,
		PaddingType paddingType,
		size_t padding)
	{
		_amountOfKernels = safety::ensureMinimum<size_t>(amountOfKernels, 1);

		// Kernel's _dimension must be at least 2.
		_dimension = safety::ensureMinimum<size_t>(dimension, 2);

		_fillType = fillType;
		_paddingType = paddingType;
		_padding = padding;
		_activationFunction = activationFunction;
		_kernelElementCount = _dimension * _dimension;

		// _stride must be at least 1 to prevent the filter from getting stuck in place.
		_stride = safety::ensureMinimum<size_t>(stride, 1);

		size_t kernelsElementCount = computeFilterDataElementCount();
		_filterData = new T[kernelsElementCount];
		memset(_filterData, 0, kernelsElementCount * sizeof(T));
	}

	template <typename T>
	inline Filter2D<T>::Filter2D(
		size_t dimension,
		size_t stride,
		ActivationFunction activationFunction,
		FillType fillType,
		PaddingType paddingType,
		size_t padding)
	{
		_amountOfKernels = 1;

		// Kernel's _dimension must be odd and at least 3.
		_dimension = safety::ensureMinimum<size_t>(dimension, 2);

		_fillType = fillType;
		_paddingType = paddingType;
		_padding = padding;
		_activationFunction = activationFunction;
		_kernelElementCount = _dimension * _dimension;

		// _stride must be at least 1 to prevent the filter from getting stuck in place.
		_stride = safety::ensureMinimum<size_t>(stride, 1);

		_filterData = nullptr;
	}

	template <typename T>
	inline Filter2D<T>::~Filter2D()
	{
		delete[] _filterData;
	}

	template <typename T>
	inline void Filter2D<T>::setFilter(T value)
	{
		// Exit if it is a subsampling filter.
		if (_filterData == nullptr)
		{
			return;
		}

		size_t kernelOffset = _dimension * _dimension;
		for (size_t kernelIndex = 0; kernelIndex < _amountOfKernels; kernelIndex++)
		{
			for (size_t j = 0; j < _dimension; j++)
			{
				for (size_t i = 0; i < _dimension; i++)
				{
					_filterData[kernelIndex * kernelOffset + i + j * _dimension] = value;
				}
			}
		}
	}

	template <typename T>
	inline void Filter2D<T>::setFilterRandom()
	{
		size_t kernelOffset = _dimension * _dimension;
		for (size_t kernelIndex = 0; kernelIndex < _amountOfKernels; kernelIndex++)
		{
			for (size_t j = 0; j < _dimension; j++)
			{
				for (size_t i = 0; i < _dimension; i++)
				{
					_filterData[kernelIndex * kernelOffset + i + j * _dimension] =
						(T)(rand() % 200 - 100) / 50;
				}
			}
		}
	}

	template <typename T>
	inline size_t Filter2D<T>::computeFilterDataElementCount()
	{
		return _amountOfKernels * _dimension * _dimension;
	}

	template <typename T>
	inline size_t Filter2D<T>::computeFilterDataSize()
	{
		return _amountOfKernels * _dimension * _dimension * sizeof(T);
	}
	
	template <typename T>
	inline size_t Filter2D<T>::computeKernelSize()
	{
		return _kernelElementCount * sizeof(T);
	}

	template <typename T>
	inline size_t Filter2D<T>::computeHalfDimension()
	{
		// Left shift is equivalent to integer devision by 2.
		return _dimension >> 1;
	}

	template <typename T>
	inline size_t Filter2D<T>::getDimension()
	{
		return _dimension;
	}

	template <typename T>
	inline size_t Filter2D<T>::getStride()
	{
		return _stride;
	}

	template <typename T>
	inline PaddingType Filter2D<T>::getPaddingType()
	{
		return _paddingType;
	}

	template <typename T>
	inline size_t Filter2D<T>::getPadding()
	{
		return _padding;
	}

	template <typename T>
	inline FillType Filter2D<T>::getFillType()
	{
		return _fillType;
	}

	template <typename T>
	inline size_t Filter2D<T>::getAmountOfKernels()
	{
		return _amountOfKernels;
	}

	template <typename T>
	inline size_t Filter2D<T>::getKernelElementCount()
	{
		return _kernelElementCount;
	}

	template <typename T>
	inline ActivationFunction Filter2D<T>::getActivationFunction()
	{
		return _activationFunction;
	}

	template <typename T>
	inline T* Filter2D<T>::getFilterData()
	{
		return _filterData;
	}
}

#endif