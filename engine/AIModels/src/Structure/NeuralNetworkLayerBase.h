#ifndef NEURALNETWORKLAYERBASE_H
#define NEURALNETWORKLAYERBASE_H

#include "Functions.h"
#include "NeuralLayerType.h"
#include "engine/CodeSafety/src/SafeArray.h"

namespace aimods
{
    template <typename T>
    class NeuralNetworkLayerBase
    {
    public:

        ActivationFunction function;
        size_t inputsCount;
        safety::SafeArray<T> outputs;

        safety::SafeArray<T> temporaryCalculations; // Data necessary for training, is allocated when calling NeuralNetworkLayerBase::prepareForTraining().
        safety::SafeArray<T> errors;                // Data necessary for training, is allocated when calling NeuralNetworkLayerBase::prepareForTraining().

        inline NeuralNetworkLayerBase(NeuralLayerType type);

        inline bool isReadyToBeTrained();
        inline NeuralLayerType getType();
        inline ActivationFunction getFunction();
        inline size_t getInputsCount();
        inline size_t getOutputsCount();
        inline size_t getIndexOfMaxOutput();

        inline void computeLastLayerErrors(safety::SafeArray<T>* targets);
        inline float computeMeanSquaredError();

        inline void setOutputs(T value);
        inline void allocateErrorsArray(); 
        inline void freeErrorsArray();
        inline void allocateTemporaryCalculations();
        inline void freeTemporaryCalculations();
        
        void printInputs(safety::SafeArray<T>* inputs);
        void printOutputs();

        virtual inline ~NeuralNetworkLayerBase();

        virtual inline void setWeightsRandom() = 0;
        virtual inline void setWeights(T value) = 0;
        virtual inline T* getWeights() = 0;
        virtual inline size_t getWeightsCount() = 0;
        virtual inline T* getThresholds() = 0;
        virtual inline void computeHiddenLayerErrors(NeuralNetworkLayerBase<T>* nextLayer) = 0;
        
        virtual inline void propagate(safety::SafeArray<T>* inputs) = 0;
        virtual inline void propagateSavingWeightedSums(safety::SafeArray<T>* inputs) = 0;
        
        virtual inline void adjustWeights(safety::SafeArray<T>* inputs, float learningRate) = 0;

    protected:

        NeuralLayerType type;

        virtual inline void computeWeightedSums(safety::SafeArray<T>* inputs, safety::SafeArray<T>* resultsArray) = 0;
        virtual inline void computeOutputs(safety::SafeArray<T>* weightedSumsArray) = 0;
    };


    // IMPLEMENTATION BELOW //


    template <typename T>
    inline NeuralNetworkLayerBase<T>::NeuralNetworkLayerBase(NeuralLayerType type)
    {
        this->function = ActivationFunction::RELU;
        this->type = type;
        this->inputsCount = 0;
    }

    template <typename T>
    inline bool NeuralNetworkLayerBase<T>::isReadyToBeTrained()
    {
        if (temporaryCalculations.getElementsCount() == 0 ||
            errors.getElementsCount() == 0)
        {
            return false;
        }
        return true;
    }

    template <typename T>
    inline NeuralLayerType NeuralNetworkLayerBase<T>::getType()
    {
        return type;
    }

    template <typename T>
    inline ActivationFunction NeuralNetworkLayerBase<T>::getFunction()
    {
        return function;
    }

    template <typename T>
    inline size_t NeuralNetworkLayerBase<T>::getInputsCount()
    {
        return inputsCount;
    }

    template <typename T>
    inline size_t NeuralNetworkLayerBase<T>::getOutputsCount()
    {
        return outputs.getElementsCount();
    }

    template <typename T>
    inline size_t NeuralNetworkLayerBase<T>::getIndexOfMaxOutput()
    {
        T maxOutputIndex = 0;
        size_t outputsCount = outputs.getElementsCount();
        for (size_t neuronIndex = 1; neuronIndex < outputsCount; neuronIndex++)
        {
            if (outputs[maxOutputIndex] < outputs[neuronIndex])
            {
                maxOutputIndex = neuronIndex;
            }
        }
        return maxOutputIndex;
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::computeLastLayerErrors(safety::SafeArray<T>* targets)
    {
        if (targets->getElementsCount() == 0 || 
            errors.getElementsCount() == 0)
        {
            return;
        }

        errors = outputs;
        errors -= *targets;
    }

    template <typename T>
    inline float NeuralNetworkLayerBase<T>::computeMeanSquaredError()
    {
        T meanSquaredError = 0.0;
        size_t outputsCount = outputs.getElementsCount();
        for (size_t neuronIndex = 0; neuronIndex < outputsCount; neuronIndex++)
        {
            meanSquaredError += std::pow(errors[neuronIndex], 2.0);
        }
        return (float)meanSquaredError / outputsCount;
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::setOutputs(T value)
    {
        outputs.setAllElements(value);
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::allocateErrorsArray()
    {
        size_t outputsCount = outputs.getElementsCount();
        errors.resize(outputsCount);
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::freeErrorsArray()
    {
        errors.deallocate();
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::allocateTemporaryCalculations()
    {
        size_t outputsCount = outputs.getElementsCount();
        temporaryCalculations.resize(outputsCount);
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::freeTemporaryCalculations()
    {
        temporaryCalculations.deallocate();
    }

    template <typename T>
    void NeuralNetworkLayerBase<T>::printInputs(safety::SafeArray<T>* inputs)
    {
        for (size_t inputIndex = 0; inputIndex < inputsCount; inputIndex++)
        {
            printf(" %3.3f", inputs->getElement(inputIndex));
        }
        printf("\n");
    }

    template <typename T>
    void NeuralNetworkLayerBase<T>::printOutputs()
    {
        size_t outputsCount = outputs.getElementsCount();
        for (size_t neuronIndex = 0; neuronIndex < outputsCount; neuronIndex++)
        {
            printf(" %3.3f", outputs[neuronIndex]);
        }
        printf("\n");
    }

    template <typename T>
    inline NeuralNetworkLayerBase<T>::~NeuralNetworkLayerBase()
    {
        outputs.deallocate();
        temporaryCalculations.deallocate();
        errors.deallocate();
    }
}

#endif