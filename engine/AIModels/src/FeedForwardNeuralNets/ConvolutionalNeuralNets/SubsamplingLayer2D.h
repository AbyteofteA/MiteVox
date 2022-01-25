
#ifndef SUBSAMPLINGLAYER2D_H
#define SUBSAMPLINGLAYER2D_H

#include "engine/AIModels/src/Structure/Functions.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/FilterLayer2DBase.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"

namespace aimods
{
	template <typename T>
	class SubsamplingLayer2D : public FilterLayer2DBase<T>
	{
	public:

		T _weight;
		T _threshold;

		SubsamplingLayer2D(
			size_t amountOfInputMaps,
			size_t inputWidth,
			size_t inputHeight,
			ActivationFunction function,
			Filter2D<T>* filter);

		inline T getWeight();
		inline T getThreshold();

		/// <summary>
		/// Propagates signals through the layer, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size FilterLayer2DBase::_inputsDataSize</param>
		inline void propagate(T* inputs);
		inline void propagateSavingWeightedSums(T* inputs);
		inline void adjustWeights(T* inputs, float learningRate) {}

	private:

		void tuneOutputs();
		inline void computeWeightedSums(T* inputs, T* resultsArray);
		inline void computeOutputs(T* weightedSumsArray);
	};


	// IMPLEMENTATION BELOW //


	template <typename T>
	SubsamplingLayer2D<T>::SubsamplingLayer2D(
		size_t amountOfInputMaps,
		size_t inputWidth,
		size_t inputHeight,
		ActivationFunction function,
		Filter2D<T>* filter) : FilterLayer2DBase<T>(
			NeuralLayerType::SUBSAMPLING_2D,
			amountOfInputMaps,
			inputWidth,
			inputHeight,
			amountOfInputMaps,
			filter)
	{
		_weight = 1;
		_threshold = 0;
		this->_function = function;
		this->tuneOutputs();
	}

	template <typename T>
	inline T SubsamplingLayer2D<T>::getWeight()
	{
		return _weight;
	}

	template <typename T>
	inline T SubsamplingLayer2D<T>::getThreshold()
	{
		return _threshold;
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::propagate(T* inputs)
	{
		computeWeightedSums(inputs, this->_outputs);
		computeWeightedSums(this->_temporaryCalculations, this->_outputs);
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::propagateSavingWeightedSums(T* inputs)
	{
		computeWeightedSums(inputs, this->_temporaryCalculations);
		computeWeightedSums(this->_temporaryCalculations, this->_outputs);
	}

	//template <typename T>
	//inline void SubsamplingLayer2D<T>::adjustWeights(T* inputs, float learningRate);

	template <typename T>
	void SubsamplingLayer2D<T>::tuneOutputs()
	{
		this->_outputMapElementCount = this->_outputWidth * this->_outputHeight;
		this->_outputsCount = this->_outputMapElementCount * this->_amountOfOutputMaps;
		this->_outputs = new T[this->_outputsCount];
		this->_outputsDataSize = this->_outputsCount * sizeof(T);
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::computeWeightedSums(T* inputs, T* resultsArray)
	{
		// Check if the inputs are passed.
		if (inputs == nullptr)
		{
			return;
		}

		// There is only one map index because amount of input maps
		// equals amount of output maps for subsampling operation.
		size_t amountOfMaps = this->getAmountOfOutputMaps();

		size_t outputHeight = this->getOutputHeight();
		size_t outputWidth = this->getOutputWidth();

		long filterHalfDimension = this->_filter->computeHalfDimension();
		long filterStride = this->_filter->getStride();

		for (size_t mapIndex = 0; mapIndex < amountOfMaps; mapIndex++)
		{
			for (size_t outputCoordY = 0; outputCoordY < outputHeight; outputCoordY++)
			{
				for (size_t outputCoordX = 0; outputCoordX < outputWidth; outputCoordX++)
				{
					// This block of code executes for every element of output map.

					T subsampledValue = 0;
					if (this->_filter->getActivationFunction() == ActivationFunction::MIN ||
						this->_filter->getActivationFunction() == ActivationFunction::MAX)
					{
						subsampledValue = inputs[0];
					}

					long filterStartOffsetFromMapsOrigin = this->_filter->getPadding() - filterHalfDimension;
					long inputCoordY = outputCoordY * filterStride - filterStartOffsetFromMapsOrigin;
					long inputCoordX = outputCoordX * filterStride - filterStartOffsetFromMapsOrigin;

					long filterRightConstraint = this->_filter->getDimension() - filterHalfDimension;
					for (long i = -filterHalfDimension; i < filterRightConstraint; i++)
					{
						for (long j = -filterHalfDimension; j < filterRightConstraint; j++)
						{
							long samplingCoordY = inputCoordY + i;
							long samplingCoordX = inputCoordX + j;

							T inputElement = this->sampleElement(samplingCoordX, samplingCoordY, mapIndex, inputs);

							switch (this->_filter->getActivationFunction())
							{
							case ActivationFunction::AVERAGE:
								subsampledValue += inputElement;
								break;

							case ActivationFunction::MIN:
								if (inputElement < subsampledValue)
								{
									subsampledValue = inputElement;
								}
								break;

							case ActivationFunction::MAX:
								if (inputElement > subsampledValue)
								{
									subsampledValue = inputElement;
								}
								break;
							}
						}
					}

					if (this->_filter->getActivationFunction() == ActivationFunction::AVERAGE)
					{
						size_t kernelElementCount = this->_filter->getKernelElementCount();
						subsampledValue /= kernelElementCount;
					}
					/// AVERAGE, d = 3, SAME
					/// 1.00 1.22 0.66 0.22
					/// 1.22 1.55 0.88 0.33
					/// 0.66 0.88 0.55 0.22
					/// 0.22 0.33 0.22 0.11
					/// 
					// Apply subsampling trainable parameters.
					subsampledValue = subsampledValue * _weight - _threshold;

					size_t outputElementIndex =
						this->computeOutputsDataIndex(outputCoordX, outputCoordY, mapIndex);
					resultsArray[outputElementIndex] = subsampledValue;
				}
			}
		}
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::computeOutputs(T* weightedSumsArray)
	{
		for (size_t outputIndex = 0; outputIndex < this->_outputsCount; outputIndex++)
		{
			weightedSumsArray[outputIndex] = activationFunction(weightedSumsArray[outputIndex], this->_function);
		}
	}
}

#endif