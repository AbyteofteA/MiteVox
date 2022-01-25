
#ifndef NEURALNETWORKDESERIALIZER_H
#define NEURALNETWORKDESERIALIZER_H

#include "NeuralNetwork.h"
#include "NeuralNetworkLayerBase.h"
#include "NeuralLayerType.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayerDeserializer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2DDeserializer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2DDeserializer.h"

#include <string>

namespace aimods
{
    template <typename T>
    NeuralNetwork<T>* NeuralNetworkFromJSON(fileio::JSON* neuralNetworkJSON)
    {
        // Deserialize general model's information.

        std::string name = neuralNetworkJSON->getFieldString("name");
        std::string documentation = neuralNetworkJSON->getFieldString("documentation");
        size_t inputsCount = (size_t)neuralNetworkJSON->getFieldNumber("inputsCount");

        NeuralNetwork<T>* neuralNetwork = new NeuralNetwork<T>(name, inputsCount);
        neuralNetwork->setDocumentation(documentation);

        // Deserialize model's layers.

        fileio::JSON* layersArrayJSON = neuralNetworkJSON->getFieldArray("layers");

        size_t layersCount = layersArrayJSON->getArraySize();
        for (size_t layerIndex = 0; layerIndex < layersCount; layerIndex++)
        {
            fileio::JSON* layer = layersArrayJSON->getArrayItem(layerIndex);
            fileio::JSON* neuralNetworkLayerBase = layer->getFieldObject("NeuralNetworkLayerBase");
            NeuralLayerType layerType = (NeuralLayerType)neuralNetworkLayerBase->getFieldNumber("type");

            // Deserialize layer depending on its type.

            switch (layerType)
            {
            case aimods::NeuralLayerType::FULLYCONNECTED:
            {
                FullyConnectedLayer<T>* fullyConnectedLayer = FullyConnectedLayerFromJSON<T>(layer);
                neuralNetwork->addLayer(fullyConnectedLayer);
                break;
            }
            case aimods::NeuralLayerType::CONVOLUTIONAL_2D:
            {
                ConvolutionalLayer2D<T>* convolutionalLayer2D = ConvolutionalLayer2DFromJSON<T>(layer);
                neuralNetwork->addLayer(convolutionalLayer2D);
                break;
            }
            case aimods::NeuralLayerType::SUBSAMPLING_2D:
            {
                SubsamplingLayer2D<T>* subsamplingLayer2D = SubsamplingLayer2DFromJSON<T>(layer);
                neuralNetwork->addLayer(subsamplingLayer2D);
                break;
            }

            //------------------------------------------------------------------------------------------

            case aimods::NeuralLayerType::SOFTMAX:
                break;

            case aimods::NeuralLayerType::CONVOLUTIONAL_1X1:
                break;

            case aimods::NeuralLayerType::CONVOLUTIONAL_DEPTHWISE_SEPARABLE_2D:
                break;

            case aimods::NeuralLayerType::CONVOLUTIONAL_DIALATED_2D:
                break;

            case aimods::NeuralLayerType::CONVOLUTIONAL_SPATIALLY_SEPARABLE_2D:
                break;

            case aimods::NeuralLayerType::DECONVOLUTIONAL_2D:
                break;

            case aimods::NeuralLayerType::UPSAMPLING_2D:
                break;

            case aimods::NeuralLayerType::HOPFIELD:
                break;
            }
        }

        return neuralNetwork;
    }

}

#endif
