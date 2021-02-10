
#ifndef AIMODELS_H
#define AIMODELS_H

// Structure
#include "AIModels/src/Structure/Functions.h"
#include "AIModels/src/Structure/LearningBatch.h"
#include "AIModels/src/Structure/FormalNeuron.h"

// FullyConnectedNeuralNets
#include "AIModels/src/FeedForwardNeuralNets/FullyConnectedLayer.h"
#include "AIModels/src/FeedForwardNeuralNets/SoftmaxLayer.h"

// ConvolutionalNeuralNets
#include "AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/FilterLayer2DBase.h"
#include "AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2D.h"
#include "AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2D.h"

// RecurrentNeuralNets
#include "AIModels/src/RecurrentNeuralNets/HopfieldNetwork.h"

#include "AIModels/src/Service.h"

// Files
#include "AIModels/src/GZ_File_IO.h"

#endif