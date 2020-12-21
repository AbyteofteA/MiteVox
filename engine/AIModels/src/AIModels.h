
#ifndef AIMODELS_H
#define AIMODELS_H

// Structure
#include "Structure\Functions.h"
#include "Structure\LearningBatch.h"
#include "Structure\HyperData.h"
#include "Structure\FormalNeuron.h"

// Perceptron
#include "FFNN\Perceptron\Perceptron.h"
#include "FFNN\Perceptron\Perceptron_Validation.h"
#include "FFNN\Perceptron\Perceptron_BackPropagation.h"
#include "FFNN\Perceptron\BackPropagationWizard.h"

// CNN
#include "FFNN\CNN\Filter_2D.h"
#include "FFNN\CNN\Convolution_layer.h"

// Hopfield network
#include "RNN\HopfieldNN.h"

#include "Service.h"

// Files
#include "FFNN\Perceptron\Perceptron_File_IO.h"
#include "GZ_File_IO.h"

#endif