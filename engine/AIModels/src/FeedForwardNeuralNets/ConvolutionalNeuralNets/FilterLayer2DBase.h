#ifndef FILTERLAYER2DBASE_H
#define FILTERLAYER2DBASE_H

#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"

#include "engine/Math/src/MinAndMax.h"
#include <algorithm>

namespace aimods
{
	template <typename T>
	class FilterLayer2DBase : public NeuralNetworkLayerBase<T>
	{
	public:

		Filter2D<T>* filter;

		inline FilterLayer2DBase(
			NeuralLayerType layerType,
			size_t amountOfInputMaps,
			size_t inputWidth,
			size_t inputHeight,
			size_t amountOfOutputMaps,
			Filter2D<T>* filter);

		inline ~FilterLayer2DBase();
		inline T* getWeights();
		inline size_t getWeightsCount();
		inline T* getThresholds();
		inline void computeHiddenLayerErrors(NeuralNetworkLayerBase<T>* nextLayer);

		/// <summary>
		/// Samples an element from the map pointed by inputs.
		/// </summary>
		/// <param name="column"> - horizontal index</param>
		/// <param name="row"> - vertical index</param>
		/// <param name="mapIndex"> - map index</param>
		/// <param name="inputsData"> - array, must be of size FilterLayer2DBase::inputsDataSize</param>
		inline T sampleElement(long column, long row, size_t mapIndex, safety::SafeArray<T>* inputsData);

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

		virtual inline void setWeightsRandom() = 0;
		virtual inline void setWeights(T value) = 0;

	protected:

		size_t inputsDataSize;
		size_t amountOfInputMaps;
		size_t inputWidth;
		size_t inputHeight;
		size_t inputMapElementCount;

		size_t outputsDataSize;
		size_t amountOfOutputMaps;
		size_t outputWidth;
		size_t outputHeight;
		size_t outputMapElementCount;

	private:

		void tuneParameters();
		virtual void tuneOutputs() = 0;
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	inline FilterLayer2DBase<T>::FilterLayer2DBase(
		NeuralLayerType layerType,
		size_t amountOfInputMaps,
		size_t inputWidth,
		size_t inputHeight,
		size_t amountOfOutputMaps,
		Filter2D<T>* filter) : NeuralNetworkLayerBase<T>(layerType)
	{
		if (filter == nullptr)
		{
			return;
		}

		this->amountOfInputMaps = std::max<size_t>(amountOfInputMaps, 1);
		this->inputWidth = std::max<size_t>(inputWidth, 1);
		this->inputHeight = std::max<size_t>(inputHeight, 1);
		this->amountOfOutputMaps = std::max<size_t>(amountOfOutputMaps, 1);
		this->inputMapElementCount = this->inputWidth * this->inputHeight;
		this->inputsCount = this->inputMapElementCount * this->amountOfInputMaps;
		this->inputsDataSize = this->inputsCount * sizeof(T);
		this->filter = filter;

		tuneParameters();
	}

	template <typename T>
	inline FilterLayer2DBase<T>::~FilterLayer2DBase()
	{
		this->outputs.deallocate();
	}

	template <typename T>
	inline T* FilterLayer2DBase<T>::getWeights()
	{
		return filter->getFilterData();
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getWeightsCount()
	{
		return filter->computeFilterDataElementCount();
	}

	template <typename T>
	inline T* FilterLayer2DBase<T>::getThresholds()
	{
		return nullptr;
	}

	template <typename T>
	inline void FilterLayer2DBase<T>::computeHiddenLayerErrors(NeuralNetworkLayerBase<T>* nextLayer)
	{
		// TODO:
	}

	template <typename T>
	inline T FilterLayer2DBase<T>::sampleElement(long column, long row, size_t mapIndex, safety::SafeArray<T>* inputsData)
	{
		if (filter == nullptr ||
			mapIndex >= amountOfInputMaps ||
			inputsData->getElementsCount() == 0)
		{
			return 0;
		}

		size_t inputMapsOffset = inputMapElementCount * mapIndex;

		switch (filter->fillType)
		{
		case FillType::ZERO:
			if (column < 0 || column >= inputWidth || row < 0 || row >= inputHeight)
			{
				return 0;
			}
			break;

		case FillType::MAX:
			if (column < 0 || column >= inputWidth || row < 0 || row >= inputHeight)
			{
				return mathem::max<T>();
			}
			break;

		case FillType::BORDER:
			if (column < 0)
			{
				column = 0;
			}
			else if (column >= inputWidth)
			{
				column = inputWidth - 1;
			}
			if (row < 0)
			{
				row = 0;
			}
			else if (row >= inputHeight)
			{
				row = inputHeight - 1;
			}
			break;

		case FillType::REPEAT:
			row = row % inputHeight;
			column = column % inputWidth;
			break;

		case FillType::REPEAT_HORIZONTALLY_ZERO:
			if (row < 0 || row >= inputHeight)
			{
				return 0;
			}
			else
			{
				column = column % inputWidth;
			}
			break;

		case FillType::REPEAT_HORIZONTALLY_MAX:
			if (row < 0 || row >= inputHeight)
			{
				return mathem::max<T>();
			}
			else
			{
				column = column % inputWidth;
			}
			break;

		case FillType::REPEAT_VERTICALLY_ZERO:
			if (column < 0 || column >= inputWidth)
			{
				return 0;
			}
			else
			{
				row = row % inputHeight;
			}
			break;

		case FillType::REPEAT_VERTICALLY_MAX:
			if (column < 0 || column >= inputWidth)
			{
				return mathem::max<T>();
			}
			else
			{
				row = row % inputHeight;
			}
			break;
		}

		return inputsData->getElement(inputMapsOffset + row * inputWidth + column);
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputsDataSize()
	{
		return inputsDataSize;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getAmountOfInputMaps()
	{
		return amountOfInputMaps;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputWidth()
	{
		return inputWidth;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputHeight()
	{
		return inputHeight;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getInputMapElementCount()
	{
		return inputMapElementCount;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputsDataSize()
	{
		return outputsDataSize;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getAmountOfOutputMaps()
	{
		return amountOfOutputMaps;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputWidth()
	{
		return outputWidth;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputHeight()
	{
		return outputHeight;
	}

	template <typename T>
	inline size_t FilterLayer2DBase<T>::getOutputMapElementCount()
	{
		return outputMapElementCount;
	}

	template <typename T>
	size_t FilterLayer2DBase<T>::computeInputsDataIndex(long column, long row, size_t mapIndex)
	{
		return inputMapElementCount * mapIndex + row * inputWidth + column;
	}

	template <typename T>
	size_t FilterLayer2DBase<T>::computeOutputsDataIndex(long column, long row, size_t mapIndex)
	{
		return outputMapElementCount * mapIndex + row * outputWidth + column;
	}

	template <typename T>
	void FilterLayer2DBase<T>::tuneParameters()
	{
		if (filter == nullptr)
		{
			return;
		}

		long deltaSize = 0;
		float outputWidthFloat = 0;
		float outputHeightFloat = 0;
		size_t filterHalfDimension = filter->computeHalfDimension();

		switch (filter->paddingType)
		{
		case PaddingType::NO_PADDING:
			outputWidth = (inputWidth - filter->dimension) / filter->stride + 1;
			outputHeight = (inputHeight - filter->dimension) / filter->stride + 1;
			break;

		case PaddingType::ARBITRARY:
			outputWidth =
				(inputWidth + filter->padding * 2 - filter->dimension) / filter->stride + 1;
			outputHeight =
				(inputHeight + filter->padding * 2 - filter->dimension) / filter->stride + 1;
			break;

		case PaddingType::SAME:
			filter->stride = 1;
			outputWidth = inputWidth;
			outputHeight = inputHeight;
			filter->padding = (long)filterHalfDimension;
			break;

		case PaddingType::DILATED:
			// TODO: implement tuning outputs for PaddingType::DILATED.
			break;
		}
	}
}

#endif