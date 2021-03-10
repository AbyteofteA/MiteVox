
#ifndef AIMODELS_H
#define AIMODELS_H

// Structure
#include "engine/AIModels/src/Structure/Functions.h"
#include "engine/AIModels/src/Structure/LearningBatch.h"
#include "engine/AIModels/src/Structure/FormalNeuron.h"

// FullyConnectedNeuralNets
#include "engine/AIModels/src/FeedForwardNeuralNets/FullyConnectedLayer.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/SoftmaxLayer.h"

// ConvolutionalNeuralNets
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/FilterLayer2DBase.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/ConvolutionalLayer2D.h"
#include "engine/AIModels/src/FeedForwardNeuralNets/ConvolutionalNeuralNets/SubsamplingLayer2D.h"

// RecurrentNeuralNets
#include "engine/AIModels/src/RecurrentNeuralNets/HopfieldNetwork.h"

#include "engine/AIModels/src/Service.h"

// Files
#include "engine/AIModels/src/GZ_File_IO.h"

#endif