
#ifndef SUBSAMPLINGLAYER2D_H
#define SUBSAMPLINGLAYER2D_H

#include "engine/AIModels/src/Structure/Functions.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/Filter2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/FilterLayer2DBase.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"

#include <cstdlib>

namespace aimods
{
	template <typename T>
	class SubsamplingLayer2D : public FilterLayer2DBase<T>
	{
	public:

		T weight;
		T threshold;

		SubsamplingLayer2D(
			size_t amountOfInputMaps,
			size_t inputWidth,
			size_t inputHeight,
			ActivationFunction function,
			Filter2D<T>* filter);

		inline void setWeightsRandom();
		inline void setWeights(T value);

		inline T getWeight();
		inline T getThreshold();

		/// <summary>
		/// Propagates signals through the layer, then stores the results in outputs.
		/// </summary>
		/// <param name="inputs"> - array, must be of size FilterLayer2DBase::inputsDataSize</param>
		inline void propagate(safety::SafeArray<T>* inputs);
		inline void propagateSavingWeightedSums(safety::SafeArray<T>* inputs);
		inline void adjustWeights(safety::SafeArray<T>* inputs, float learningRate);

	private:

		void tuneOutputs();
		inline void computeWeightedSums(safety::SafeArray<T>* inputs, safety::SafeArray<T>* resultsArray);
		inline void computeOutputs(safety::SafeArray<T>* weightedSumsArray);
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
		weight = 1.0f;
		threshold = 0.0f;
		this->function = function;
		this->tuneOutputs();
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::setWeightsRandom()
	{
		weight = (T)rand() / ((T)RAND_MAX * 10.0f);
		threshold = (T)rand() / ((T)RAND_MAX * 10.0f);
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::setWeights(T value)
	{
		weight = value;
		threshold = value;
	}

	template <typename T>
	inline T SubsamplingLayer2D<T>::getWeight()
	{
		return weight;
	}

	template <typename T>
	inline T SubsamplingLayer2D<T>::getThreshold()
	{
		return threshold;
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::propagate(safety::SafeArray<T>* inputs)
	{
		computeWeightedSums(inputs, &this->outputs);
		computeOutputs(&this->outputs);
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::propagateSavingWeightedSums(safety::SafeArray<T>* inputs)
	{
		computeWeightedSums(inputs, &this->temporaryCalculations);
		computeOutputs(&this->temporaryCalculations);
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::adjustWeights(safety::SafeArray<T>* inputs, float learningRate)
	{
		// TODO: 
	}

	template <typename T>
	void SubsamplingLayer2D<T>::tuneOutputs()
	{
		size_t outputMapElementCount = this->outputWidth * this->outputHeight;
		size_t outputsCount = outputMapElementCount * this->amountOfOutputMaps;
		this->outputs.resize(outputsCount);
		this->outputsDataSize = outputsCount * sizeof(T);
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::computeWeightedSums(safety::SafeArray<T>* inputs, safety::SafeArray<T>* resultsArray)
	{
		if (inputs->getElementsCount() == 0)
		{
			return;
		}

		// There is only one map index because amount of input maps
		// equals amount of output maps for subsampling operation.
		size_t amountOfMaps = this->getAmountOfOutputMaps();

		size_t outputHeight = this->getOutputHeight();
		size_t outputWidth = this->getOutputWidth();

		long filterHalfDimension = this->filter->computeHalfDimension();
		long filterStride = this->filter->getStride();

		for (size_t mapIndex = 0; mapIndex < amountOfMaps; mapIndex++)
		{
			for (size_t outputCoordY = 0; outputCoordY < outputHeight; outputCoordY++)
			{
				for (size_t outputCoordX = 0; outputCoordX < outputWidth; outputCoordX++)
				{
					// This block of code executes for every element of output map.

					T subsampledValue = 0;
					if (this->function == ActivationFunction::MIN ||
						this->function == ActivationFunction::MAX)
					{
						subsampledValue = inputs->getElement(0);
					}

					long filterStartOffsetFromMapsOrigin = this->filter->getPadding() - filterHalfDimension;
					long inputCoordY = outputCoordY * filterStride - filterStartOffsetFromMapsOrigin;
					long inputCoordX = outputCoordX * filterStride - filterStartOffsetFromMapsOrigin;

					long filterRightConstraint = this->filter->getDimension() - filterHalfDimension;
					for (long i = -filterHalfDimension; i < filterRightConstraint; i++)
					{
						for (long j = -filterHalfDimension; j < filterRightConstraint; j++)
						{
							long samplingCoordY = inputCoordY + i;
							long samplingCoordX = inputCoordX + j;

							T inputElement = this->sampleElement(samplingCoordX, samplingCoordY, mapIndex, inputs);

							switch (this->function)
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

					if (this->function == ActivationFunction::AVERAGE)
					{
						size_t kernelElementCount = this->filter->getKernelElementCount();
						subsampledValue /= kernelElementCount;
					}
					/// AVERAGE, d = 3, SAME
					/// 1.00 1.22 0.66 0.22
					/// 1.22 1.55 0.88 0.33
					/// 0.66 0.88 0.55 0.22
					/// 0.22 0.33 0.22 0.11
					/// 
					// Apply subsampling trainable parameters.
					subsampledValue = subsampledValue * weight - threshold;

					size_t outputElementIndex =
						this->computeOutputsDataIndex(outputCoordX, outputCoordY, mapIndex);
					resultsArray[outputElementIndex] = subsampledValue;
				}
			}
		}
	}

	template <typename T>
	inline void SubsamplingLayer2D<T>::computeOutputs(safety::SafeArray<T>* weightedSumsArray)
	{
		size_t outputsCount = this->outputs.getElementsCount();
		for (size_t outputIndex = 0; outputIndex < outputsCount; outputIndex++)
		{
			T weightedSum = weightedSumsArray->getElement(outputIndex);
			weightedSumsArray->setElement(outputIndex, activationFunction(weightedSum, this->function));
		}
	}
}

#endif