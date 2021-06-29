
#ifndef NEURALNETWORKLAYERBASE_H
#define NEURALNETWORKLAYERBASE_H

#include "Functions.h"
#include "NeuralLayerType.h"

namespace aimods
{
    template <typename T>
    class NeuralNetworkLayerBase
    {
    public:

        ActivationFunction _function;

        inline NeuralNetworkLayerBase(NeuralLayerType type);

        inline bool isReadyToBeTrained();
        inline NeuralLayerType getType();
        inline ActivationFunction getFunction();
        inline size_t getInputsCount();
        inline size_t getOutputsCount();
        inline T* getOutputs();
        inline size_t getMaxOutputIndex();
        inline T* getTemporaryCalculations();

        inline T* getErrors();
        
        /// <summary>
        /// Computes errors for the last layer.
        /// </summary>
        /// <param name="targets">- target values for the outputs;</param>
        inline void computeLastLayerErrors(T* targets);

        inline float computeMeanSquaredError();

        inline void setOutputs(T value);
        inline void allocateErrorsArray();
        inline void freeErrorsArray();
        inline void allocateTemporaryCalculations();
        inline void freeTemporaryCalculations();

        void printInputs(T* inputs);
        void printOutputs();

        virtual inline ~NeuralNetworkLayerBase();

        virtual inline void setWeightsRandom() = 0;
        virtual inline void setWeights(T value) = 0;
        virtual inline T* getWeights() = 0;
        virtual inline size_t getWeightsCount() = 0;
        virtual inline T* getThresholds() = 0;

        /// <summary>
        /// Computes errors for the hidden layers.
        /// </summary>
        /// <param name="nextLayer">- target values for the outputs;</param>
        virtual inline void computeHiddenLayerErrors(NeuralNetworkLayerBase<T>* nextLayer) = 0;

        virtual inline void propagate(T* inputs) = 0;
        virtual inline void propagateSavingWeightedSums(T* inputs) = 0;

        virtual inline void adjustWeights(T* inputs, float learningRate) = 0;

    protected:

        NeuralLayerType _type;

        size_t _inputsCount;
        size_t _outputsCount;
        T* _outputs;

        // Data necessary for training, is allocated when calling NeuralNetworkLayerBase::prepareForTraining().

        T* _temporaryCalculations;
        T* _errors;

        virtual inline void computeWeightedSums(T* inputs, T* resultsArray) = 0;
        virtual inline void computeOutputs(T* weightedSumsArray) = 0;
    };


    // IMPLEMENTATION BELOW //


    template <typename T>
    inline NeuralNetworkLayerBase<T>::NeuralNetworkLayerBase(NeuralLayerType type)
    {
        _function = ActivationFunction::RELU;
        _type = type;
        _inputsCount = 0;
        _outputsCount = 0;
        _outputs = nullptr;
        _temporaryCalculations = nullptr;
        _errors = nullptr;
    }

    template <typename T>
    inline bool NeuralNetworkLayerBase<T>::isReadyToBeTrained()
    {
        if (_temporaryCalculations == nullptr || _errors == nullptr)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    template <typename T>
    inline NeuralLayerType NeuralNetworkLayerBase<T>::getType()
    {
        return _type;
    }

    template <typename T>
    inline ActivationFunction NeuralNetworkLayerBase<T>::getFunction()
    {
        return _function;
    }

    template <typename T>
    inline size_t NeuralNetworkLayerBase<T>::getInputsCount()
    {
        return _inputsCount;
    }

    template <typename T>
    inline size_t NeuralNetworkLayerBase<T>::getOutputsCount()
    {
        return _outputsCount;
    }

    template <typename T>
    inline T* NeuralNetworkLayerBase<T>::getOutputs()
    {
        return _outputs;
    }

    template <typename T>
    inline size_t NeuralNetworkLayerBase<T>::getMaxOutputIndex()
    {
        T maxOutputIndex = 0;
        for (size_t neuronIndex = 1; neuronIndex < this->_outputsCount; neuronIndex++)
        {
            if (_outputs[maxOutputIndex] < _outputs[neuronIndex])
            {
                maxOutputIndex = neuronIndex;
            }
        }
        return maxOutputIndex;
    }

    template <typename T>
    inline T* NeuralNetworkLayerBase<T>::getTemporaryCalculations()
    {
        return _temporaryCalculations;
    }

    template <typename T>
    inline T* NeuralNetworkLayerBase<T>::getErrors()
    {
        return _errors;
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::computeLastLayerErrors(T* targets)
    {
        // Exit if targets are not passed.
        if (targets == nullptr)
        {
            return;
        }

        // Exit if errors array is not allocated.
        if (this->_errors == nullptr)
        {
            return;
        }

        for (size_t neuronIndex = 0; neuronIndex < this->_outputsCount; neuronIndex++)
        {
            this->_errors[neuronIndex] = this->_outputs[neuronIndex] - targets[neuronIndex];
        }
    }

    template <typename T>
    inline float NeuralNetworkLayerBase<T>::computeMeanSquaredError()
    {
        T meanSquaredError = 0;
        for (size_t neuronIndex = 0; neuronIndex < this->_outputsCount; neuronIndex++)
        {
            meanSquaredError += pow(this->_errors[neuronIndex], 2);
        }
        return (float)meanSquaredError / this->_outputsCount;
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::setOutputs(T value)
    {
        for (size_t neuronIndex = 0; neuronIndex < this->_outputsCount; neuronIndex++)
        {
            this->_outputs[neuronIndex] = value;
        }
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::allocateErrorsArray()
    {
        this->_errors = new T[this->_outputsCount];
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::freeErrorsArray()
    {
        delete[] this->_errors;
        this->_errors = nullptr;
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::allocateTemporaryCalculations()
    {
        this->_temporaryCalculations = new T[this->_outputsCount];
    }

    template <typename T>
    inline void NeuralNetworkLayerBase<T>::freeTemporaryCalculations()
    {
        delete[] this->_temporaryCalculations;
        this->_temporaryCalculations = nullptr;
    }

    template <typename T>
    void NeuralNetworkLayerBase<T>::printInputs(T* inputs)
    {
        for (size_t inputIndex = 0; inputIndex < _inputsCount; inputIndex++)
        {
            printf(" %3.3f", inputs[inputIndex]);
        }
        printf("\n");
    }

    template <typename T>
    void NeuralNetworkLayerBase<T>::printOutputs()
    {
        for (size_t neuronIndex = 0;
            neuronIndex < _outputsCount;
            neuronIndex++)
        {
            printf(" %3.3f", this->_outputs[neuronIndex]);
        }
        printf("\n");
    }

    template <typename T>
    inline NeuralNetworkLayerBase<T>::~NeuralNetworkLayerBase()
    {

    }
}

#endif