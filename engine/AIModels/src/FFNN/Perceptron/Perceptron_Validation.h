
#ifndef PERCEPTRON_VALIDATION_H
#define PERCEPTRON_VALIDATION_H

#include "AIModels/src/FFNN/Perceptron/Perceptron.h"

namespace aimods
{

	float Perceptron::Validate(LearningBatch** testBatch, unsigned int batchSize)
	{
		if (this->amountOfLayers == 0)
			return -1;

		int misses = 0;
		int strikes = 0;
		float result = 0;

		for (unsigned int s = 0; s < batchSize; s++)
		{
			for (unsigned int b = 0; b < testBatch[s]->batchSize; b++)
			{
				inputData(testBatch[s]->input[b]);
				propagate();

				int answerIndex = 0;

				for (unsigned int n = 0; n < amountOfLayerNeurons[amountOfLayers - 1]; n++)
				{
					if (testBatch[s]->output[b][n] > testBatch[s]->output[b][answerIndex])
						answerIndex = n;
				}

				if (this->netResponse() == answerIndex)
					strikes++;
				else
					misses++;
			}
		}
		return (float)strikes / (misses + strikes);
	}

}

#endif