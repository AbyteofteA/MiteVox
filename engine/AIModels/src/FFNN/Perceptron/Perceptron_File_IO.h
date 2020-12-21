
#ifndef PERCEPTRON_FILE_IO_H
#define PERCEPTRON_FILE_IO_H

/*-----------------------------------------------------------------------------------
	NNSH(.nnsh) file structure:
-------------------------------------------------------------------------------------
			bytes			|			type
-------------------------------------------------------------------------------------
4							|		unsigned int amountOfLayers
4 * amountOfLayers			|		unsigned int * amountOfLayerNeurons
4 * amountOfLayers			|		unsigned int * activtionFunctions
							|			
							|		{		// For every neuron //
							|			unsigned int amountOfAxons
Sum(amountOfLayerNeurons) * |			float output
* (12 + 4 * amountOfAxons)	|			float threshold
							|			float function
							|
							|			{		// For every axon //
							|				float * weightArray
							|			}
							|		}
-----------------------------------------------------------------------------------
	NOTE: It supports fully-connected networks only. (TO FIX)
-----------------------------------------------------------------------------------*/

#include "AIModels/src/FFNN/Perceptron/Perceptron.h"
#include "AIModels/src/Service.h"

#include <iostream>
#include <ctime>


namespace aimods
{

	/*****************************************************************************************
	Generates a name for a file based on date and time.
	*****************************************************************************************/
	char* Perceptron::nameAFile()
	{
		time_t t = time(NULL);
		struct tm myTime;
		localtime_s(&myTime, &t);

		const int nameSize = 35;

		char* filename = (char*)malloc(sizeof(char) * nameSize);
		char prefix[] = "NN-snapshot";
		unsigned int i = 0;
		while (prefix[i] != '\0')
		{
			filename[i] = prefix[i];
			i++;
		}

		int postfixSize = nameSize - 11;
		char* postfix = (char*)malloc(sizeof(char) * postfixSize);
		sprintf_s(postfix, postfixSize, "(%02d.%02d.%04d %02d'%02d)", myTime.tm_mday, myTime.tm_mon + 1,
			myTime.tm_year + 1900, myTime.tm_hour, myTime.tm_min);
		while (postfix[i - 11] != '\0')
		{
			filename[i] = postfix[i - 11];
			i++;
		}
		// Forming the extansion
		filename[nameSize - 6] = '.';
		filename[nameSize - 5] = 'n';
		filename[nameSize - 4] = 'n';
		filename[nameSize - 3] = 's';
		filename[nameSize - 2] = 'h';
		filename[nameSize - 1] = '\0';
		i = 0;

		printf("\n Creating a file:  ");
		while (filename[i] != '\0')
		{
			printf("%c", filename[i]);
			i++;
		}
		printf("\n");

		return filename;
	}


	void Perceptron::writeTo_NNSH(char* filename = NULL)
	{
		if (filename == NULL)
			filename = nameAFile();

		FILE* newFile;
		fopen_s(&newFile, filename, "wb");
		if (newFile == NULL)
		{
			printf("\n ERROR! Cannot open the file.\n");
			return;
		}
		else
		{
			printf("\n Writing the file..\n");
			fwrite(&(amountOfLayers), sizeof(unsigned int), 1, newFile);
			fwrite(amountOfLayerNeurons, sizeof(unsigned int), amountOfLayers, newFile);

			for (unsigned int i = 0; i < amountOfLayers; i++)
			{
				for (unsigned int j = 0; j < amountOfLayerNeurons[i]; j++)
				{
					fwrite(&((layerArray[i])[j]->amountOfAxons), sizeof(unsigned int), 1, newFile);
					fwrite(&((layerArray[i])[j]->output), sizeof(float), 1, newFile);
					fwrite(&((layerArray[i])[j]->threshold), sizeof(float), 1, newFile);
					fwrite(&((layerArray[i])[j]->function), sizeof(unsigned char), 1, newFile);
					fwrite((layerArray[i])[j]->weightArray, sizeof(float),
						(layerArray[i])[j]->amountOfAxons, newFile);
				}
			}
		}
		fclose(newFile);
	}


	void Perceptron::readFrom_NNSH(char* filename)
	{
		FILE* newFile;
		fopen_s(&newFile, filename, "rb");
		if (newFile == NULL)
		{
			printf("\n ERROR! Cannot open the file.\n");
			return;
		}
		else
		{
			printf("\n Reading the file..\n");
			unsigned int amountOfLayers = 0;

			fread(&amountOfLayers, sizeof(unsigned int), 1, newFile);
			unsigned int* amountOfLayerNeurons = (unsigned int*)malloc(sizeof(unsigned int*) * amountOfLayers);
			fread(amountOfLayerNeurons, sizeof(unsigned int), amountOfLayers, newFile);

			for (unsigned int i = 0; i < amountOfLayers; i++)
			{
				addLayer(amountOfLayerNeurons[i], TanH);
			}

			for (unsigned int i = 0; i < amountOfLayers; i++)
			{
				for (unsigned int j = 0; j < amountOfLayerNeurons[i]; j++)
				{
					FormalNeuron* newNeuron = new FormalNeuron();
					fread(&(newNeuron->amountOfAxons), sizeof(unsigned int), 1, newFile);
					fread(&(newNeuron->output), sizeof(float), 1, newFile);
					fread(&(newNeuron->threshold), sizeof(float), 1, newFile);
					fread(&(newNeuron->function), sizeof(unsigned char), 1, newFile);
					newNeuron->weightArray = (float*)malloc(sizeof(float) * newNeuron->amountOfAxons);
					fread(newNeuron->weightArray, sizeof(float), newNeuron->amountOfAxons, newFile);
					newNeuron->axonArray = (FormalNeuron**)malloc(sizeof(FormalNeuron**) * newNeuron->amountOfAxons);

					*((layerArray[i])[j]) = *newNeuron;
					delete newNeuron;
					if (i != 0)
					{
						for (unsigned int k = 0; k < (layerArray[i])[j]->amountOfAxons; k++)
							(layerArray[i])[j]->axonArray[k] = (layerArray[i - 1])[k];
					}
				}
			}
		}

		fclose(newFile);
	}


	char* Perceptron::writeTo_CHAR()
	{
		char* bytes = (char*)malloc(sizeof(char) * fileSize);
		unsigned int pointer = 0;

		printf("\n Writing the file..\n");

		memcpy(bytes + pointer, &amountOfLayers, sizeof(unsigned int));
		pointer += sizeof(unsigned int);
		for (unsigned int i = 0; i < amountOfLayers; i++)
		{
			memcpy(bytes + pointer, &amountOfLayerNeurons[i], sizeof(unsigned int));
			pointer += sizeof(unsigned int);
		}

		for (unsigned int i = 0; i < amountOfLayers; i++)
		{
			for (unsigned int j = 0; j < amountOfLayerNeurons[i]; j++)
			{
				memcpy(bytes + pointer, &(layerArray[i])[j]->amountOfAxons, sizeof(unsigned int));
				pointer += sizeof(unsigned int);
				memcpy(bytes + pointer, &(layerArray[i])[j]->output, sizeof(float));
				pointer += sizeof(float);
				memcpy(bytes + pointer, &(layerArray[i])[j]->threshold, sizeof(float));
				pointer += sizeof(float);
				memcpy(bytes + pointer, &(layerArray[i])[j]->function, sizeof(unsigned char));
				pointer += sizeof(unsigned char);
				for (unsigned int k = 0; k < (layerArray[i])[j]->amountOfAxons; k++)
				{
					memcpy(bytes + pointer, &(layerArray[i])[j]->weightArray[k], sizeof(float));
					pointer += sizeof(float);
				}
			}
		}

		return bytes;
	}


	void Perceptron::readFrom_CHAR(char* bytes)
	{
		deleteNetwork();

		char* currentByte = bytes;
		printf("\n Reading the file..\n");
		unsigned int amountOfLayers = ((unsigned int*)currentByte)[0];
		currentByte += sizeof(unsigned int);
		for (unsigned int i = 0; i < amountOfLayers; i++)
		{
			addLayer(((unsigned int*)currentByte)[0], TanH);
			currentByte += sizeof(unsigned int);
		}

		for (unsigned int i = 0; i < amountOfLayers; i++)
		{
			for (unsigned int j = 0; j < amountOfLayerNeurons[i]; j++)
			{
				FormalNeuron* newNeuron = new FormalNeuron();
				newNeuron->amountOfAxons = ((unsigned int*)currentByte)[0];
				currentByte += sizeof(unsigned int);
				newNeuron->output = ((float*)currentByte)[0];
				currentByte += sizeof(float);
				newNeuron->threshold = ((float*)currentByte)[0];
				currentByte += sizeof(float);
				newNeuron->function = ((unsigned char*)currentByte)[0];
				currentByte += sizeof(unsigned char);

				newNeuron->weightArray = (float*)malloc(sizeof(float) * newNeuron->amountOfAxons);
				for (unsigned int k = 0; k < newNeuron->amountOfAxons; k++)
				{
					newNeuron->weightArray[k] = ((float*)currentByte)[0];
					currentByte += sizeof(float);
				}
				newNeuron->axonArray = (FormalNeuron**)malloc(sizeof(FormalNeuron**) * newNeuron->amountOfAxons);

				*((layerArray[i])[j]) = *newNeuron;
				delete newNeuron;
				if (i != 0)
				{
					for (unsigned int k = 0; k < (layerArray[i])[j]->amountOfAxons; k++)
						(layerArray[i])[j]->axonArray[k] = (layerArray[i - 1])[k];
				}
			}
		}

	}
}


#endif
