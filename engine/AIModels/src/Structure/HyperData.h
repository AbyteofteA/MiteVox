
#ifndef HYPERDATA_H
#define HYPERDATA_H

#include "AIModels/src/Structure/LearningBatch.h"

#include <iostream>
#include <cstdarg>

namespace aimods
{
	enum struct hyperdataType { BYTE = 0, SBYTE, SHORT, INT, FLOAT, DOUBLE };

	template <typename T>
	class HyperData
	{
	public:

		hyperdataType type = hyperdataType::BYTE;
		unsigned char numOfDimentions = 0;
		size_t* sizeOfDimentions = nullptr;

		T* data = nullptr;

		HyperData(hyperdataType datType = hyperdataType::BYTE)
		{
			type = datType;
			numOfDimentions = 0;
			sizeOfDimentions = nullptr;
		}

		void resize(unsigned char numOfDims, ...)
		{
			std::va_list args;
			va_start(args, numOfDims);

			numOfDimentions = numOfDims;
			free(sizeOfDimentions);
			sizeOfDimentions = (size_t*)realloc(
				sizeOfDimentions, sizeof(size_t) * numOfDimentions);

			size_t volume = 1;
			for (int i = 0; i < numOfDimentions; i++)
			{
				size_t tmpArg = va_arg(args, size_t);
				volume *= tmpArg;
				sizeOfDimentions[i] = tmpArg;
			}
			va_end(args);

			data = (T*)realloc(data, sizeof(T) * volume);
		}

		HyperData<T>* copy()
		{
			HyperData<T>* newHyperData = new HyperData<T>(type, numOfDimentions, sizeOfDimentions, nullptr);
			size_t volume = getVolume();
			T* dat = (T*)malloc(sizeof(T) * volume);
			for (int i = 0; i < volume; i++)
			{
				dat[i] = data[i];
			}
			newHyperData->data = dat;

			// you should copy sizeOfDims too.

			return newHyperData;
		}

		void* readFrom_IDX(char* filename)
		{
			FILE* newFile = nullptr;
			fopen_s(&newFile, filename, "rb");

			if (newFile == nullptr)
			{
				printf("\n ERROR! Cannot open .idx file.\n");
				return nullptr;
			}
			else
			{
				printf("\n Reading .idx file..");
				printf("\n %s\n [", filename);

				unsigned char type = 0;
				fread(&type, sizeof(char), 1, newFile);	// Zero
				if (type != 0) return nullptr;
				fread(&type, sizeof(char), 1, newFile);	// Zero
				if (type != 0) return nullptr;
				fread(&type, sizeof(char), 1, newFile);	// Data type

				unsigned char numOfDims = 0;
				fread(&numOfDims, sizeof(char), 1, newFile);	// Number of dimension
				unsigned int* sizeOfDims = (unsigned int*)malloc(sizeof(unsigned int) * numOfDims);
				unsigned int volume = 1;
				for (int i = 0; i < numOfDims; i++)
				{
					unsigned char tmp1 = 0;
					unsigned char tmp2 = 0;
					unsigned char tmp3 = 0;
					unsigned char tmp4 = 0;
					fread(&tmp1, sizeof(unsigned char), 1, newFile);
					fread(&tmp2, sizeof(unsigned char), 1, newFile);
					fread(&tmp3, sizeof(unsigned char), 1, newFile);
					fread(&tmp4, sizeof(unsigned char), 1, newFile);
					sizeOfDims[i] = 0;
					sizeOfDims[i] |= tmp4;
					sizeOfDims[i] |= ((unsigned int)tmp3) << 8;
					sizeOfDims[i] |= ((unsigned int)tmp2) << 16;
					sizeOfDims[i] |= ((unsigned int)tmp1) << 24;
					volume *= sizeOfDims[i];
				}

				resize(numOfDims, sizeOfDims);

				T tmp = 0;
				float progress = 0;
				for (unsigned int i = 0; i < volume; i++)
				{
					fread(&tmp, sizeof(T), 1, newFile);
					data[i] = tmp;

					progress++;
					if ((progress / (float)volume * 100) >= 5)
					{
						progress = 0;
						printf("#");
					}
				}
				printf("]\n");
			}

			fclose(newFile);
			return this;
		}

		unsigned char getDataType()
		{
			return type;
		}

		unsigned char getNumOfDimentions()
		{
			return numOfDimentions;
		}

		unsigned int* getSizeOfDimentions()
		{
			return sizeOfDimentions;
		}

		unsigned int getDimention(unsigned char indx)
		{
			if (indx < numOfDimentions)
			{
				return sizeOfDimentions[indx];
			}
			else
			{
				return 0;
			}
		}

		size_t getVolume()
		{
			size_t volume = 1;
			for (size_t i = 0; i < numOfDimentions; i++)
				volume *= sizeOfDimentions[i];
			return volume;
		}

		/*******************************************************************************
			1 dims:	i
			2 dims:	(j * sizeI) + i
			3 dims:	(k * sizeJ * sizeI) + (j * sizeI) + i
			4 dims:	(l * sizeK * sizeJ * sizeI) + (k * sizeJ * sizeI) + (j * sizeI) + i
		*******************************************************************************/
		T get(unsigned int* indeces)
		{
			unsigned int index = 0;
			unsigned int tmp = 1;
			if (numOfDimentions > 1)
			{
				for (int i = numOfDimentions - 1; i >= 1; i--)
				{
					tmp *= sizeOfDimentions[i];
					index += tmp * indeces[i - 1];
				}

				index += indeces[numOfDimentions - 1];
			}
			else
				index = indeces[0];

			return data[index];
		}

		void set(unsigned int* indeces, T value)
		{
			unsigned int index = 0;
			unsigned int tmp = 1;
			if (numOfDimentions > 1)
			{
				for (int i = numOfDimentions - 1; i >= 1; i--)
				{
					tmp *= sizeOfDimentions[i];
					index += tmp * indeces[i - 1];
				}

				index += indeces[numOfDimentions - 1];
			}
			else
				index += indeces[0];

			data[index] = value;
		}



		~HyperData()
		{
			delete sizeOfDimentions;
			sizeOfDimentions = nullptr;
			delete data;
			data = nullptr;
		}
	};


	template <typename T>
	LearningBatch** divideIntoBatches(HyperData<T>* dataInputs, HyperData<T>* dataOutputs, unsigned int batchSize, unsigned int amOfBatches, unsigned int sizeOfOutput)
	{

		if ((dataInputs->getNumOfDimentions() != 3) && (dataOutputs->getNumOfDimentions() != 3))
			return nullptr;

		if ((dataInputs->getDimention(0) / batchSize) < amOfBatches)
			amOfBatches = (dataInputs->getDimention(2) / batchSize);


		LearningBatch** batchArray = (LearningBatch**)malloc(sizeof(LearningBatch*) * amOfBatches);
		for (unsigned int b = 0; b < amOfBatches; b++)
		{
			unsigned int sizeOfInput = dataInputs->getDimention(1) * dataInputs->getDimention(2);
			batchArray[b] = new LearningBatch(batchSize, sizeOfInput, sizeOfOutput);

			for (unsigned int j = 0; j < batchArray[b]->batchSize; j++)
			{
				for (unsigned int i = 0; i < batchArray[b]->sizeOfInput; i++)
				{
					unsigned char tmpGet = dataInputs->data[i + j * batchArray[b]->sizeOfInput + b * batchArray[b]->batchSize * batchArray[b]->sizeOfInput];
					if (tmpGet != 0)
						batchArray[b]->input[j][i] = tmpGet / (float)255;
					else
						batchArray[b]->input[j][i] = 0;
					/*else
						batchArray[b]->input[j][i] = (rand() % 1000) /(float)5000;*/
				}
				batchArray[b]->output[j][dataOutputs->data[j + b * batchArray[b]->batchSize]] = 1.;
			}
		}

		return batchArray;
	}

}

#endif