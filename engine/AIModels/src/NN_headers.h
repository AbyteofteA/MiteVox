#pragma once

#include <iostream>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <conio.h>


	// Structure
#include "Structure\Functions.h"
#include "Structure\LearningBatch.h"
#include "Structure\HyperData.h"
	// CNN
#include "CNN\Filter_2D.h"
#include "CNN\Convolution_layer.h"
	// FFNN
#include "FFNN\FormalNeuron.h"
using namespace aimods;
#include "FFNN\FFNN.h"
	// RNN
#include "RNN\HopfieldNN.h"

#include "Service.h"

	// Files
#include "FFNN\FFNN_File_IO.h"
#include "GZ_File_IO.h"

#include "FFNN\FFNN_Validation.h"
#include "FFNN\FFNN_BackPropagation.h"
#include "FFNN\BackPropagationWizard.h"
