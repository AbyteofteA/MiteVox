#ifndef NEURALNETWORKCODEC_H
#define NEURALNETWORKCODEC_H

#include "engine/FileIO/src/Formats/JSON/JSON.h"
#include "engine/AIModels/src/Structure/NeuralNetwork.h"
#include "engine/AIModels/src/Structure/NeuralNetworkLayerBase.h"
#include "engine/AIModels/src/Structure/NeuralLayerType.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2D.h"

#include "engine/AIModels/src/NeuralNetworkCodec/FullyConnectedLayerCodec.h"
#include "engine/AIModels/src/NeuralNetworkCodec/ConvolutionalLayer2DCodec.h"
#include "engine/AIModels/src/NeuralNetworkCodec/SubsamplingLayer2DCodec.h"

#include <string>

namespace aimods
{
    class NeuralNetworkCodec
    {
    public:

        template <typename T>
        static NeuralNetwork<T>* fromJSON(fileio::JSON* neuralNetworkJSON);

        template <typename T>
        static fileio::JSON* toJSON(NeuralNetwork<T>* neuralNetwork);
    };

    template <typename T>
    NeuralNetwork<T>* NeuralNetworkCodec::fromJSON(fileio::JSON* neuralNetworkJSON)
    {
        std::string name = neuralNetworkJSON->getFieldString("name");
        std::string documentation = neuralNetworkJSON->getFieldString("documentation");
        size_t inputsCount = (size_t)neuralNetworkJSON->getFieldNumber("inputsCount");

        NeuralNetwork<T>* neuralNetwork = new NeuralNetwork<T>(name, inputsCount);
        neuralNetwork->setDocumentation(documentation);

        fileio::JSON* layersArrayJSON = neuralNetworkJSON->getFieldArray("layers");

        size_t layersCount = layersArrayJSON->getArraySize();
        for (size_t layerIndex = 0; layerIndex < layersCount; layerIndex++)
        {
            fileio::JSON* layer = layersArrayJSON->getArrayItem(layerIndex);
            fileio::JSON* neuralNetworkLayerBase = layer->getFieldObject("NeuralNetworkLayerBase");
            NeuralLayerType layerType = 
                stringToNeuralLayerType(neuralNetworkLayerBase->getFieldStringOrDefault("type", "INVALID").c_str());

            switch (layerType)
            {
            case aimods::NeuralLayerType::FULLYCONNECTED:
            {
                FullyConnectedLayer<T>* fullyConnectedLayer = FullyConnectedLayerCodec::fromJSON<T>(layer);
                neuralNetwork->addLayer(fullyConnectedLayer);
                break;
            }
            case aimods::NeuralLayerType::CONVOLUTIONAL_2D:
            {
                ConvolutionalLayer2D<T>* convolutionalLayer2D = ConvolutionalLayer2DCodec::fromJSON<T>(layer);
                neuralNetwork->addLayer(convolutionalLayer2D);
                break;
            }
            case aimods::NeuralLayerType::SUBSAMPLING_2D:
            {
                SubsamplingLayer2D<T>* subsamplingLayer2D = SubsamplingLayer2DCodec::fromJSON<T>(layer);
                neuralNetwork->addLayer(subsamplingLayer2D);
                break;
            }

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

    template <typename T>
    fileio::JSON* NeuralNetworkCodec::toJSON(NeuralNetwork<T>* neuralNetwork)
    {
        fileio::JSON* json = new fileio::JSON();

        json->setField("name", neuralNetwork->getName());
        json->setField("documentation", neuralNetwork->getDocumentation());
        json->setField("inputsCount", (double)neuralNetwork->getInputsCount());
        
        fileio::JSON* layersArray = json->setFieldType("layers", fileio::JSONtype::ARRAY);
        
        size_t layersCount = neuralNetwork->getLayersCount();
        for (size_t layerIndex = 0; layerIndex < layersCount; layerIndex++)
        {
            NeuralNetworkLayerBase<T>* layer = neuralNetwork->getLayer(layerIndex);

            switch (layer->getType())
            {
            case aimods::NeuralLayerType::FULLYCONNECTED:
            {
                fileio::JSON* fullyConnectedLayer = 
                    FullyConnectedLayerCodec::toJSON<T>((FullyConnectedLayer<T>*)layer);
                layersArray->addArrayItem(fullyConnectedLayer);
                break;
            }
            case aimods::NeuralLayerType::CONVOLUTIONAL_2D:
            {
                fileio::JSON* convolutionalLayer2D = 
                    ConvolutionalLayer2DCodec::toJSON<T>((ConvolutionalLayer2D<T>*)layer);
                layersArray->addArrayItem(convolutionalLayer2D);
                break;
            }
            case aimods::NeuralLayerType::SUBSAMPLING_2D:
            {
                fileio::JSON* subsamplingLayer2D = 
                    SubsamplingLayer2DCodec::toJSON<T>((SubsamplingLayer2D<T>*)layer);
                layersArray->addArrayItem(subsamplingLayer2D);
                break;
            }
            
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
