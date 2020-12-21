
#ifndef BACKPROPAGATIONWIZARD_H
#define BACKPROPAGATIONWIZARD_H

#include "AIModels/src/Structure/LearningBatch.h"
#include "AIModels/src/FFNN/Perceptron/Perceptron.h"

namespace aimods
{

	void BackPropagationWizard(Perceptron* net, LearningBatch** learningData, unsigned int learningDataSize, LearningBatch** testData, unsigned int testDataSize, float acceptableError = 0.1)
	{
		float currentError = 0;
		unsigned int batch = 0;
		bool stop = false;
		float strikesPercentage = 0;

		while (batch < learningDataSize)
		{
			currentError = net->BackPropagation_STEP(learningData[batch], 0.01f);

			system("cls");

			printf("\n\t[batch = %d]\n", batch);
			printf("\tCurrent error = %.3f\n\tStrikes percentage [", currentError);

			int iMax = (int)(30 * strikesPercentage);
			for (int i = 0; i < 30; i++)
			{
				if (i <= iMax)
					printf("#");
				else
					printf("_");
			}
			printf("] %.3f %%\n", (float)strikesPercentage * (float)100);


			strikesPercentage = net->Validate(testData, testDataSize);

			if (strikesPercentage > (1 - acceptableError))
				break;

			batch++;
		}
	}
}

#endif