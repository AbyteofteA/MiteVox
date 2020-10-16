
#include "NN_headers.h"


void main()
{
	showTitle();
	srand((unsigned int)time(NULL));

	aimods::HyperData<unsigned char>* learningInputs = new aimods::HyperData<unsigned char>();
	aimods::HyperData<unsigned char>* learningOutputs = new aimods::HyperData<unsigned char>();
	aimods::HyperData<unsigned char>* testInputs = new aimods::HyperData<unsigned char>();
	aimods::HyperData<unsigned char>* testOutputs = new aimods::HyperData<unsigned char>();

	// Type MNIST dataset's file path here.
	learningInputs->readFrom_IDX("");
	learningOutputs->readFrom_IDX("");
	testInputs->readFrom_IDX("");
	testOutputs->readFrom_IDX("");

	LearningBatch** learningData = divideIntoBatches<unsigned char>(learningInputs, learningOutputs, 1, 60000, 10);
	LearningBatch** testData = divideIntoBatches<unsigned char>(testInputs, testOutputs, 50, 1, 10);

	Feedforward_NN* testNet = new Feedforward_NN();
	testNet->addLayer(784, TanH);
	testNet->addLayer(100, TanH);
	testNet->addLayer(10, TanH);
	
	BackPropagationWizard(testNet, learningData, 60000, testData, 1, 0.25);
	testNet->writeTo_NNSH("FFNN_MNIST_75.nnsh");

	printf("\n\tValidation(testNet)  = %.3f", testNet->Validate(testData, 1));

	getchar();
}