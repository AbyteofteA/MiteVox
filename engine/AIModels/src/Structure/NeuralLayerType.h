
#ifndef LAYERTYPE_H
#define LAYERTYPE_H

namespace aimods
{
    enum struct NeuralLayerType
    {
        INVALID = 0,
        FULLYCONNECTED,                         // [X] [X]
        SOFTMAX,                                // [X] [ ]
        CONVOLUTIONAL_2D,                       // [ ] [ ]
        CONVOLUTIONAL_SPATIALLY_SEPARABLE_2D,   // TODO: Implement
        CONVOLUTIONAL_DEPTHWISE_SEPARABLE_2D,   // TODO: Implement
        CONVOLUTIONAL_DIALATED_2D,              // TODO: Implement
        CONVOLUTIONAL_1X1,                      // TODO: Implement
        SUBSAMPLING_2D,                         // [ ] [ ]
        DECONVOLUTIONAL_2D,                     // TODO: Implement
        UPSAMPLING_2D,                          // TODO: Implement
        HOPFIELD,                               // TODO: Implement
    };

    const char* neuralLayerTypeToString(NeuralLayerType neuralLayerType);
    NeuralLayerType stringToNeuralLayerType(const char* string);
}

#endif