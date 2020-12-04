#pragma once

namespace aimods
{
	/*				dataType's possible values:
	**************************************************************************/
#define UNSIGNED_BYTE_HYPERDATA		0x08	// unsigned byte	| 1 byte
#define SIGNED_BYTE_HYPERDATA		0x09	// signed byte		| 1 byte
#define SHORT_HYPERDATA				0x0B	// short			| 2 bytes
#define INT_HYPERDATA				0x0C	// int				| 4 bytes
#define FLOAT_HYPERDATA				0x0D	// float			| 4 bytes
#define DOUBLE_HYPERDATA			0x0E	// double			| 8 bytes


	template <typename Type>
	class HyperData
	{
	public:

		unsigned char dataType = 0;
		unsigned char numOfDimentions = 0;
		unsigned int* sizeOfDimentions = NULL;

		Type* data = NULL;

		HyperData()
		{
			dataType = 0;
			numOfDimentions = 0;
			sizeOfDimentions = NULL;
		}
		HyperData(unsigned char datType, unsigned char numOfDims, unsigned int* sizeOfDims, Type* dat)
		{
			dataType = datType;
			numOfDimentions = numOfDims;
			sizeOfDimentions = sizeOfDims;
			data = dat;
		}

		void resize(unsigned char numOfDims, unsigned int* _sizeOfDimentions)
		{
			delete sizeOfDimentions;
			sizeOfDimentions = NULL;

			numOfDimentions = numOfDims;
			sizeOfDimentions = _sizeOfDimentions;

			unsigned long long volume = 1;
			for (int i = 0; i < numOfDims; i++)
				volume *= _sizeOfDimentions[i];
			data = (Type*)realloc(data, sizeof(Type) * (size_t)volume);
		}

		HyperData<Type>* copy()
		{
			HyperData<Type>* newHyperData = new HyperData<Type>(dataType, numOfDimentions, sizeOfDimentions, NULL);
			unsigned int volume = getVolume();
			Type* dat = (Type*)malloc(sizeof(Type) * volume);
			for (int i = 0; i < volume; i++)
			{
				dat[i] = data[i];
			}
			newHyperData->data = dat;

			// you should copy sizeOfDims too.

			return newHyperData;
		}

		/*void loadFromImage(char* filename)
		{
			dataType = FLOAT_HYPERDATA;

			sf::Image* img = new sf::Image();
			img->loadFromFile(filename);

			unsigned int* _sizeOfDims = (unsigned int*)malloc(sizeof(unsigned int) * 3);
			_sizeOfDims[0] = 3;
			_sizeOfDims[1] = img->getSize().y;
			_sizeOfDims[2] = img->getSize().x;

			resize(3, _sizeOfDims);

			for (unsigned int j = 0; j < img->getSize().y; j++)
			{
				for (unsigned int i = 0; i < img->getSize().x; i++)
				{
					data[(i + j * img->getSize().x) + 0 * img->getSize().x * img->getSize().y] = img->getPixel(i, j).r;
					data[(i + j * img->getSize().x) + 1 * img->getSize().x * img->getSize().y] = img->getPixel(i, j).g;
					data[(i + j * img->getSize().x) + 2 * img->getSize().x * img->getSize().y] = img->getPixel(i, j).b;
				}
			}

			delete img;
		}*/

		void* readFrom_IDX(char* filename)
		{
			//FILE* newFile;
			//void* hyperData;

			FILE* newFile = nullptr;
			fopen_s(&newFile, filename, "rb");

			if (newFile == NULL)
			{
				printf("\n ERROR! Cannot open .idx file.\n");
				return NULL;
			}
			else
			{
				printf("\n Reading .idx file..");
				printf("\n %s\n [", filename);

				unsigned char type = 0;
				fread(&type, sizeof(char), 1, newFile);	// Zero
				if (type != 0) return NULL;
				fread(&type, sizeof(char), 1, newFile);	// Zero
				if (type != 0) return NULL;
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

				Type tmp = 0;
				float progress = 0;
				for (unsigned int i = 0; i < volume; i++)
				{
					fread(&tmp, sizeof(Type), 1, newFile);
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

		/*sf::Image* saveToImage(char* filename)
		{
			sf::Image* img = new sf::Image();

			img->create(getDimention(2), getDimention(1), sf::Color(0, 0, 0, 0));

			for (unsigned int j = 0; j < getDimention(1); j++)
			{
				for (unsigned int i = 0; i < getDimention(2); i++)
				{
					unsigned int R = 0;
					unsigned int G = 0;
					unsigned int B = 0;

					if (getDimention(0) == 1)		// grayscale image
					{
						R = data[(i + j * img->getSize().x)];
						G = R;
						B = R;
					}
					else if (getDimention(0) == 2)	// 2 channels
					{
						R = data[(i + j * img->getSize().x) + 0 * img->getSize().x * img->getSize().y];
						G = 0;
						B = data[(i + j * img->getSize().x) + 1 * img->getSize().x * img->getSize().y];
					}
					else if (getDimention(0) == 3)	// RGB image
					{
						R = data[(i + j * img->getSize().x) + 0 * img->getSize().x * img->getSize().y];
						G = data[(i + j * img->getSize().x) + 1 * img->getSize().x * img->getSize().y];
						B = data[(i + j * img->getSize().x) + 2 * img->getSize().x * img->getSize().y];
					}

					sf::Color color = sf::Color(R, G, B, 255);
					img->setPixel(i, j, color);
				}
			}

			img->saveToFile(filename);
			return img;
		}*/

		/*void slice(unsigned char dimIndx)
		{
			if (dimIndx >= numOfDimentions)
				return;
			if (dimIndx <= 1) // ???
				return;


			HyperData<Type>** result = (HyperData<Type>**)malloc(sizeof(HyperData<Type>*) * sizeOfDimentions[dimIndx]);
			for (int i = 0; i < numOfDimentions; i++)
			{
				result[i] = new HyperData<Type>();
				result[i]->resize(numOfDimentions - 1, );
			}

			unsigned char last = dimIndx;
			unsigned char first = 0;
			if (last != numOfDimentions - 1)
				first = numOfDimentions - 1;
			else
				first = numOfDimentions - 2;
			unsigned int offset = 1;
			for (int i = 0; i < numOfDimentions; i++)
			{
				if (i != last)
					offset *= sizeOfDimentions[i];
			}


			unsigned int* indeces = (unsigned int*)calloc(numOfDimentions, sizeof(unsigned int));
			for (int k = 0; k < sizeOfDimentions[dimIndx]; k++)
			{


				unsigned int index = 0;

				if (numOfDimentions > 1)
				{
					index += k * offset;
					unsigned int sizeIndex = 1;
					unsigned int indexIndex = 0;
					for (int i = 0; i < numOfDimentions - 1; i++)
					{
						while (sizeIndex == first || sizeIndex == last)
							sizeIndex++;
						if (i != first && i != last)
						{
							tmp *= sizeOfDimentions[sizeIndex];
							index += tmp * indeces[indexIndex];
							sizeIndex++;
						}
						indexIndex++;
					}
					index += indeces[last];
				}
				else
					index = indeces[0];



			}

		}*/

		unsigned char getDataType()
		{
			return dataType;
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

		unsigned long long getVolume()
		{
			unsigned long long volume = 1;
			for (int i = 0; i < numOfDimentions; i++)
				volume *= sizeOfDimentions[i];
			return volume;
		}

		/*******************************************************************************
			1 dims:	i
			2 dims:	(j * sizeI) + i
			3 dims:	(k * sizeJ * sizeI) + (j * sizeI) + i
			4 dims:	(l * sizeK * sizeJ * sizeI) + (k * sizeJ * sizeI) + (j * sizeI) + i
		*******************************************************************************/
		Type get(unsigned int* indeces)
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

		void set(unsigned int* indeces, Type value)
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
			sizeOfDimentions = NULL;
			delete data;
			data = NULL;
		}
	};


	template <typename Type>
	LearningBatch** divideIntoBatches(HyperData<Type>* dataInputs, HyperData<Type>* dataOutputs, unsigned int batchSize, unsigned int amOfBatches, unsigned int sizeOfOutput)
	{

		if ((dataInputs->getNumOfDimentions() != 3) && (dataOutputs->getNumOfDimentions() != 3))
			return NULL;

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