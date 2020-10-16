#pragma once

namespace aimods
{

	float Feedforward_NN::BackPropagation_STEP(LearningBatch* batch, float learningRate)
	{
		float* localError = (float*)calloc(amountOfLayerNeurons[amountOfLayers - 1], sizeof(float));
		float resultError = 0;

		printf("\t");

		for (unsigned int b = 0; b < batch->batchSize; b++)	
		{
			inputData(batch->input[b]);
			propagate();

			int answerIndex = 0;

			for (unsigned int n = 0; n < amountOfLayerNeurons[amountOfLayers - 1]; n++)	
			{
				firstErrors[n] = layerArray[amountOfLayers - 1][n]->output - batch->output[b][n];
				localError[n] = pow((float)layerArray[amountOfLayers - 1][n]->output - batch->output[b][n], (float)2);

				if (batch->output[b][n] > batch->output[b][answerIndex])
					answerIndex = n;
			}
		}

		printf("\n");

		// Process the last layer.
		for (unsigned int n = 0; n < amountOfLayerNeurons[amountOfLayers - 1]; n++)	
		{
			float weighSum = layerArray[amountOfLayers - 1][n]->weightedSum();
			float der = activationDerivative(weighSum, layerArray[amountOfLayers - 1][n]->function);
			dE_dz[amountOfLayers - 1][n] = firstErrors[n] * der;

			for (unsigned int a = 0; a < layerArray[amountOfLayers - 1][n]->amountOfAxons; a++)
			{
				errors[amountOfLayers - 1][n][a] =
					dE_dz[amountOfLayers - 1][n] * layerArray[amountOfLayers - 1][n]->weightArray[a];
			}

			localError[n] /= (2 * batch->batchSize);
		}

		// Process the hidden layers.
		for (unsigned int l = amountOfLayers - 2; l >= 1; l--)	
		{
			for (unsigned int n = 0; n < amountOfLayerNeurons[l]; n++)	
			{
				float weighSum = layerArray[l][n]->weightedSum();
				float der = activationDerivative(weighSum, layerArray[l][n]->function);
				float tmpErr = 0;

				for (unsigned int n1 = 0; n1 < amountOfLayerNeurons[l + 1]; n1++)
				{
					tmpErr += errors[l + 1][n1][n];
				}
				dE_dz[l][n] = tmpErr * der;

				for (unsigned int a = 0; a < layerArray[l][n]->amountOfAxons; a++)
				{
					errors[l][n][a] = dE_dz[l][n] * layerArray[l][n]->weightArray[a];
				}
			}
		}

		// Update the weights and thresholds.
		for (unsigned int l = amountOfLayers - 1; l >= 1; l--)	
		{
			for (unsigned int n = 0; n < amountOfLayerNeurons[l]; n++)	
			{
				for (unsigned int a = 0; a < layerArray[l][n]->amountOfAxons; a++)
				{
					layerArray[l][n]->weightArray[a] -=
						dE_dz[l][n] * layerArray[l][n]->axonArray[a]->output * learningRate;
				}
				layerArray[l][n]->threshold -= dE_dz[l][n] * learningRate;
			}
		}

		for (unsigned int n = 0; n < amountOfLayerNeurons[amountOfLayers - 1]; n++)
		{
			resultError += localError[n];
		}

		return resultError;
	}
}