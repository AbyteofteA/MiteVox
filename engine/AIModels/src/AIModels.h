
#ifndef AIMODELS_H
#define AIMODELS_H

// Structure
#include "AIModels/src/Structure/Functions.h"
#include "AIModels/src/Structure/LearningBatch.h"
#include "AIModels/src/Structure/FormalNeuron.h"

// FFNN
#include "AIModels/src/FFNN/FullyConnectedLayer.h"
#include "AIModels/src/FFNN/SoftmaxLayer.h"

// CNN
#include "AIModels/src/FFNN/CNN/FilterLayer2DBase.h"
#include "AIModels/src/FFNN/CNN/ConvolutionalLayer2D.h"
#include "AIModels/src/FFNN/CNN/SubsamplingLayer2D.h"

// RNN
#include "AIModels/src/RNN/HopfieldNetwork.h"

#include "AIModels/src/Service.h"

// Files
#include "AIModels/src/GZ_File_IO.h"

#endif