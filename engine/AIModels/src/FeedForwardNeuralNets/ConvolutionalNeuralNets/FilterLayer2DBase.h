
#ifndef FILTERLAYER2DBASE_H
#define FILTERLAYER2DBASE_H

#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"

#include <cstdlib>
#include <cstring>
#include <limits>

namespace aimods
{
	template <typename T>
	class FilterLayer2DBase : public NeuralNetworkLayerBase<T>
	{
	public:

		Filter2D<T>* _filter;

		inline FilterLayer2DBase(
			NeuralLayerType layerType,
			size_t amountOfInputMaps,
			size_t inputWidth,
			size_t inputHeight,
			Filter2D<T>* filter);

		inline ~FilterLayer2DBase();
		inline void setWeightsRandom();
		inline void setWeights(T value);
		inline T* getWeights();
		inline size_t getWeightsCount();
		inline T* getThresholds();
		inline void computeHiddenLayerErrors(NeuralNetworkLayerBase<T>* nextLayer);

		virtual inline void propagate(T* inputs) = 0;
		virtual inline void propagateSavingWeightedSums(T* inputs) = 0;
		virtual inline void adjustWeights(T* inputs, float learningRate) = 0;

		/// <summary>
		/// Samples an element from the map pointed by inputs.
		/// </summary>
		/// <param name="column"> - horizontal index</param>
		/// <param name="row"> - vertical index</param>
		/// <param name="mapIndex"> - map index</param>
		/// <param name="inputsData"> - array, must be of size FilterLayer2DBase::_inputsDataSize</param>
		inline T sampleElement(long column, long row, size_t mapIndex, T* inputsData);

		size_t computeInputsDataIndex(long column, long row, size_t mapIndex);
		size_t computeOutputsDataIndex(long column, long row, size_t mapIndex);

		// Getters //

		inline size_t getInputsDataSize();
		inline size_t getAmountOfInputMaps();
		inline size_t getInputWidth();
		inline size_t getInputHeight();
		inline size_t getInputMapElementCount();

		inline size_t getOutputsDataSize();
		inline size_t getAmountOfOutputMaps();
		inline size_t getOutputWidth();
		inline size_t getOutputHeight();
		inline size_t getOutputMapElementCount();

	private:

		size_t _inputsDataSize;
		size_t _amountOfInputMaps;
		size_t _inputWidth;
		size_t _inputHeight;
		size_t _inputMapElementCount;

		size_t _outputsDataSize;
		size_t _amountOfOutputMaps;
		size_t _outputWidth;
		size_t _outputHeight;
		size_t _outputMapElementCount;

		void tuneOutputs();

		virtual inline void computeWeightedSums(T* inputs, T* resultsArray) = 0;
		virtual inline void computeOutputs(T* weightedSumsArray) = 0;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline FilterLayer2DBase<T>::FilterLayer2DBase(
		NeuralLayerType layerType,
		size_t amountOfInputMaps,
		size_t inputWidth,
		size_t inputHeight,
		Filter2D<T>* filter) : NeuralNetworkLayerBase<T>(layerType)
	{
		// Check if the filter is passed.
		if (filter == nullptr)
		{
			return;
		}

		_amountOfInputMaps = safety::ensureMinimum<size_t>(amountOfInputMaps, 1);
		_inputWidth = safety::ensureMinimum<size_t>(inputWidth, 1);
		_inputHeight = safety::ensureMinimum<size_t>(inputHeight, 1);
		_inputMapElementCount = _inputWidth * _inputHeight;
		this->_inputsCount = _inputMapElementCount * _amountOfInputMaps;
		_inputsDataSize = this->_inputsCount * sizeof(T);
		_filter = filter;

		tuneOutputs();
	}

	template <typename T>
	inline FilterLayer2DBase<T>::~FilterLayer2DBase()
	{
		delete[] this->_outputs;
	}

	template <typename T>
	inline void FilterLayer2DBase<T>::setWeightsRandom()
	{
		this->_filter->setFilterRandom();
	}

	template <typename T>
	inline void FilterLayer2DBase<T>::setWeights(T value)
	{
		this->_filter->setFilter(value);
	}

	template <typename T>
	inline T* FilterLayer2DBase<T>::getWeights()
	{
		return _filter->getFilterData();
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getWeightsCount()
	{
		return _filter->computeFilterDataElementCount();
	}

	template <typename T>
	inline T* FilterLayer2DBase<T>::getThresholds()
	{
		return nullptr;
	}

	template <typename T>
	inline void FilterLayer2DBase<T>::computeHiddenLayerErrors(NeuralNetworkLayerBase<T>* nextLayer)
	{

	}

	template <typename T>
	inline T FilterLayer2DBase<T>::sampleElement(long column, long row, size_t mapIndex, T* inputsData)
	{
		// Check if the filter exists.
		if (_filter == nullptr)
		{
			return 0;
		}

		// mapIndex must be valid (less than _amountOfInputMaps).
		if (mapIndex >= _amountOfInputMaps)
		{
			return 0;
		}

		// Check if the inputs are passed.
		if (inputsData == nullptr)
		{
			return 0;
		}

		size_t inputMapsOffset = _inputMapElementCount * mapIndex;

		switch (_filter->_fillType)
		{
		case FillType::ZERO:
			if (column < 0 || column >= _inputWidth || row < 0 || row >= _inputHeight)
			{
				return 0;
			}
			else
			{
				return inputsData[inputMapsOffset + row * _inputWidth + column];
			}

		case FillType::MAX:
			if (column < 0 || column >= _inputWidth || row < 0 || row >= _inputHeight)
			{
				return std::numeric_limits<T>().max();
			}
			else
			{
				return inputsData[inputMapsOffset + row * _inputWidth + column];
			}

		case FillType::BORDER:
			if (column < 0)
			{
				column = 0;
			}
			else if (column >= _inputWidth)
			{
				column = _inputWidth - 1;
			}
			if (row < 0)
			{
				row = 0;
			}
			else if (row >= _inputHeight)
			{
				row = _inputHeight - 1;
			}
			return inputsData[inputMapsOffset + row * _inputWidth + column];

		case FillType::REPEAT:
			row = row % _inputHeight;
			column = column % _inputWidth;
			return inputsData[inputMapsOffset + row * _inputWidth + column];

		case FillType::REPEAT_HORIZONTALLY_ZERO:
			if (row < 0 || row >= _inputHeight)
			{
				return 0;
			}
			else
			{
				column = column % _inputWidth;
				return inputsData[inputMapsOffset + row * _inputWidth + column];
			}

		case FillType::REPEAT_HORIZONTALLY_MAX:
			if (row < 0 || row >= _inputHeight)
			{
				return std::numeric_limits<T>().max();
			}
			else
			{
				column = column % _inputWidth;
				return inputsData[inputMapsOffset + row * _inputWidth + column];
			}

		case FillType::REPEAT_VERTICALLY_ZERO:
			if (column < 0 || column >= _inputWidth)
			{
				return 0;
			}
			else
			{
				row = row % _inputHeight;
				return inputsData[inputMapsOffset + row * _inputWidth + column];
			}

		case FillType::REPEAT_VERTICALLY_MAX:
			if (column < 0 || column >= _inputWidth)
			{
				return std::numeric_limits<T>().max();
			}
			else
			{
				row = row % _inputHeight;
				return inputsData[inputMapsOffset + row * _inputWidth + column];
			}
		}

		return 0;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputsDataSize()
	{
		return _inputsDataSize;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getAmountOfInputMaps()
	{
		return _amountOfInputMaps;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputWidth()
	{
		return _inputWidth;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputHeight()
	{
		return _inputHeight;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputMapElementCount()
	{
		return _inputMapElementCount;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputsDataSize()
	{
		return _outputsDataSize;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getAmountOfOutputMaps()
	{
		return _amountOfOutputMaps;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputWidth()
	{
		return _outputWidth;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputHeight()
	{
		return _outputHeight;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputMapElementCount()
	{
		return _outputMapElementCount;
	}

	template <typename T>
	void FilterLayer2DBase<T>::tuneOutputs()
	{
		// Check if the filter exists.
		if (_filter == nullptr)
		{
			return;
		}

		long deltaSize = 0;
		float outputWidthFloat = 0;
		float outputHeightFloat = 0;
		size_t filterHalfDimension = _filter->computeHalfDimension();

		switch (_filter->_paddingType)
		{
		case PaddingType::NO_PADDING:
			_outputWidth = (_inputWidth - _filter->_dimension) / _filter->_stride + 1;
			_outputHeight = (_inputHeight - _filter->_dimension) / _filter->_stride + 1;
			break;

		case PaddingType::ARBITRARY:
			_outputWidth =
				(_inputWidth + _filter->_padding * 2 - _filter->_dimension) / _filter->_stride + 1;
			_outputHeight =
				(_inputHeight + _filter->_padding * 2 - _filter->_dimension) / _filter->_stride + 1;
			break;

		case PaddingType::SAME:
			_filter->_stride = 1;
			_outputWidth = _inputWidth;
			_outputHeight = _inputHeight;
			_filter->_padding = (long)filterHalfDimension;
			break;

		case PaddingType::DILATED:
			// TODO: implement tuning outputs for PaddingType::DILATED.
			break;
		}

		_outputMapElementCount = _outputWidth * _outputHeight;
		_amountOfOutputMaps = _amountOfInputMaps * _filter->_amountOfKernels;
		this->_outputsCount = _outputMapElementCount * _amountOfOutputMaps;
		this->_outputs = new T[this->_outputsCount];
		_outputsDataSize = this->_outputsCount * sizeof(T);
	}

	template <typename T>
	size_t FilterLayer2DBase<T>::computeInputsDataIndex(long column, long row, size_t mapIndex)
	{
		return 
			_inputMapElementCount * mapIndex +
			row * _inputWidth + 
			column;
	}

	template <typename T>
	size_t FilterLayer2DBase<T>::computeOutputsDataIndex(long column, long row, size_t mapIndex)
	{
		return
			_outputMapElementCount * mapIndex +
			row * _outputWidth +
			column;
	}
}

#endif