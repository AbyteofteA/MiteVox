
#ifndef NEURALNETWORKSERIALIZER_H
#define NEURALNETWORKSERIALIZER_H

#include "NeuralNetwork.h"
#include "NeuralNetworkLayerBase.h"
#include "NeuralLayerType.h"
#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "NeuralNetworkLayerBaseSerializer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/FilterLayer2DBaseSerializer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayerSerializer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2DSerializer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2DSerializer.h"

#include <string>

namespace aimods
{
    template <typename T>
    fileio::JSON* NeuralNetworkToJSON(NeuralNetwork<T>* neuralNetwork)
    {
        fileio::JSON* json = new fileio::JSON();

        // Serialize general model's information.

        json->setField("name", neuralNetwork->getName());
        json->setField("documentation", neuralNetwork->getDocumentation());
        json->setField("inputsCount", (double)neuralNetwork->getInputsCount());
        
        // Serialize model's layers.

        fileio::JSON* layersArray = json->setFieldType("layers", fileio::JSONtype::ARRAY);
        
        size_t layersCount = neuralNetwork->getLayersCount();
        for (size_t layerIndex = 0; layerIndex < layersCount; layerIndex++)
        {
            NeuralNetworkLayerBase<T>* layer = neuralNetwork->getLayer(layerIndex);

            // Serialize layer depending on its type and put it in the layer array.

            switch (layer->getType())
            {
            case aimods::NeuralLayerType::FULLYCONNECTED:
            {
                fileio::JSON* fullyConnectedLayer = 
                    FullyConnectedLayerToJSON<T>((FullyConnectedLayer<T>*)layer);
                layersArray->addArrayItem(fullyConnectedLayer);
                break;
            }
            case aimods::NeuralLayerType::CONVOLUTIONAL_2D:
            {
                fileio::JSON* convolutionalLayer2D = 
                    ConvolutionalLayer2DToJSON<T>((ConvolutionalLayer2D<T>*)layer);
                layersArray->addArrayItem(convolutionalLayer2D);
                break;
            }
            case aimods::NeuralLayerType::SUBSAMPLING_2D:
            {
                fileio::JSON* subsamplingLayer2D = 
                    SubsamplingLayer2DToJSON<T>((SubsamplingLayer2D<T>*)layer);
                layersArray->addArrayItem(subsamplingLayer2D);
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

        return json;
    }

}

#endif
