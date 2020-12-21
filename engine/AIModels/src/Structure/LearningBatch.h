
#ifndef LEARNINGBATCH_H
#define LEARNINGBATCH_H

#include <iostream>

namespace aimods
{

	class LearningBatch
	{
	public:

		unsigned int batchSize = 0;

		float** input = NULL;
		unsigned int sizeOfInput = 0;

		float** output = NULL;
		unsigned int sizeOfOutput = 0;


		LearningBatch()
		{
			batchSize = 0;
			input = NULL;
			sizeOfInput = 0;
			output = NULL;
			sizeOfOutput = 0;
		}

		LearningBatch(unsigned int btchSize, unsigned int sizeOfIn, unsigned int sizeOfOut)
		{
			batchSize = btchSize;
			sizeOfInput = sizeOfIn;
			sizeOfOutput = sizeOfOut;
			input = (float**)calloc(batchSize, sizeof(float*));
			output = (float**)calloc(batchSize, sizeof(float*));
			for (unsigned int j = 0; j < batchSize; j++)
			{
				input[j] = (float*)calloc(sizeOfInput, sizeof(float));
				output[j] = (float*)calloc(sizeOfOutput, sizeof(float));
			}
		}

		void init(float* _input, float* _output)
		{
			for (unsigned int j = 0; j < batchSize; j++)
			{
				for (unsigned int i = 0; i < sizeOfInput; i++)
				{
					input[j][i] = _input[j * batchSize + i];
				}
				for (unsigned int i = 0; i < sizeOfOutput; i++)
				{
					output[j][i] = _output[j * batchSize + i];
				}
			}
		}
		void init(float** _input, float** _output)
		{
			for (unsigned int j = 0; j < batchSize; j++)
			{
				for (unsigned int i = 0; i < sizeOfInput; i++)
				{
					input[j][i] = _input[j][i];
				}
				for (unsigned int i = 0; i < sizeOfOutput; i++)
				{
					output[j][i] = _output[j][i];
				}
			}
		}

		~LearningBatch()
		{
			for (unsigned int j = 0; j < batchSize; j++)
			{
				free(input[j]);
				free(output[j]);
			}
			free(input);
			free(output);

			batchSize = 0;
			sizeOfInput = 0;
			sizeOfOutput = 0;
		}
	};

}

#endif