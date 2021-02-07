
#ifndef HYPERDATA_H
#define HYPERDATA_H

#include <iostream>
#include <cstdarg>

namespace mathem
{
	enum struct hyperdataType { BYTE = 0, SBYTE, SHORT, INT, FLOAT, DOUBLE };

	template <typename T>
	class HyperData
	{

	public:

		hyperdataType paddingType = hyperdataType::BYTE;
		unsigned char numOfDimentions = 0;
		size_t* sizeOfDimentions = nullptr;

		T* data = nullptr;

		HyperData(hyperdataType datType = hyperdataType::BYTE)
		{
			paddingType = datType;
			numOfDimentions = 0;
			sizeOfDimentions = nullptr;
		}
		~HyperData()
		{
			delete sizeOfDimentions;
			sizeOfDimentions = nullptr;
			delete data;
			data = nullptr;
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
			HyperData<T>* newHyperData = new HyperData<T>(paddingType, numOfDimentions, sizeOfDimentions, nullptr);
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

				unsigned char paddingType = 0;
				fread(&paddingType, sizeof(char), 1, newFile);	// Zero
				if (paddingType != 0) return nullptr;
				fread(&paddingType, sizeof(char), 1, newFile);	// Zero
				if (paddingType != 0) return nullptr;
				fread(&paddingType, sizeof(char), 1, newFile);	// Data paddingType

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
			return paddingType;
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
	};
}

#endif