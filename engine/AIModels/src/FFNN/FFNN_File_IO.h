#pragma once


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


/*****************************************************************************************
Generates a name for a file based on date and time.
*****************************************************************************************/
char* Feedforward_NN::nameAFile()
{
	time_t t = time(NULL);
	struct tm myTime;
	localtime_s(&myTime, &t);

	const int nameSize = 35;

	char* filename = (char*)malloc(sizeof(char) * nameSize);
	char* prefix = "NN-snapshot";
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


void Feedforward_NN::writeTo_NNSH(char* filename = NULL)
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


void Feedforward_NN::readFrom_NNSH(char* filename)
{
	FILE * newFile;
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


char* Feedforward_NN::writeTo_CHAR()
{
	char* bytes = (char*)malloc(sizeof(char) * fileSize);
	unsigned int pointer = 0;

	printf("\n Writing the file..\n");

	writeBinaryToString<unsigned int>(bytes, &pointer, amountOfLayers);
	for (unsigned int i = 0; i < amountOfLayers; i++)
	{
		writeBinaryToString<unsigned int>(bytes, &pointer, amountOfLayerNeurons[i]);
	}

	for (unsigned int i = 0; i < amountOfLayers; i++)
	{
		for (unsigned int j = 0; j < amountOfLayerNeurons[i]; j++)
		{
			writeBinaryToString<unsigned int>(bytes, &pointer, (layerArray[i])[j]->amountOfAxons);
			writeBinaryToString<float>(bytes, &pointer, (layerArray[i])[j]->output);
			writeBinaryToString<float>(bytes, &pointer, (layerArray[i])[j]->threshold);
			writeBinaryToString<unsigned char>(bytes, &pointer, (layerArray[i])[j]->function);
			for (unsigned int k = 0; k < (layerArray[i])[j]->amountOfAxons; k++)
			{
				writeBinaryToString<float>(bytes, &pointer, (layerArray[i])[j]->weightArray[k]);
			}
		}
	}
	
	return bytes;
}


void Feedforward_NN::readFrom_CHAR(char* bytes)
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



/*-----------------------------------------------------------------------------------
	Import file format:
-------------------------------------------------------------------------------------
		<type>		|	Data type. Supports <int> & <char>.
-------------------------------------------------------------------------------------
		{ , , }		|	Input pattern bounded by curly braces.
-------------------------------------------------------------------------------------
		   @		|	End of import character.
-------------------------------------------------------------------------------------
	NOTE: 
-----------------------------------------------------------------------------------*/


void** inputFromTxt(char* filename)
{
	if (filename == NULL)
	{
		printf("\n ERROR! Filename is not found.\n");
		return NULL;
	}

	void** inputData = NULL;
	FILE* newFile;
	fopen_s(&newFile, filename, "r");

	if (newFile == NULL)
	{
		printf("\n ERROR! Cannot open the file.\n");
		return NULL;
	}
	else
	{
		char* typeStr = NULL;
		char tmpChar = getc(newFile);

		//printf("\n%c\n", tmpChar);

		while (tmpChar != EOF)
		{
			if (tmpChar == '<')
			{
				tmpChar = getc(newFile);
				while (tmpChar != '>')
				{
					typeStr = addChar2Str(typeStr, tmpChar);
					tmpChar = getc(newFile);
				}
				break;
			}
			tmpChar = getc(newFile);
		}

		unsigned int amountOfPatterns = 0;

		switch (whichStr(typeStr))
		{

		case INT_STR:
			{
				while ((tmpChar != EOF) && (tmpChar != '@'))
				{
					unsigned int sizeOfPattern = 0;
					if (tmpChar == '{')
					{
						sizeOfPattern = 0;
						amountOfPatterns++;
						inputData = (void** )realloc(inputData, sizeof(void**) * (amountOfPatterns + 1));
						inputData[amountOfPatterns - 1] = NULL;
						inputData[amountOfPatterns] = '\0';

						tmpChar = getc(newFile);
						char * tmpStr = NULL;

						while (tmpChar != '}')
						{
							switch (tmpChar)
							{

							case ',':
								{
									sizeOfPattern++;
									int tmpInt = strtol(tmpStr, NULL, 0);
									free(tmpStr);
									tmpStr = NULL;

									inputData[amountOfPatterns - 1] = (void*)realloc(inputData[amountOfPatterns - 1], sizeof(int* ) * (sizeOfPattern + 1));
									((int*)inputData[amountOfPatterns - 1])[sizeOfPattern - 1] = tmpInt;
									((int*)inputData[amountOfPatterns - 1])[sizeOfPattern] = ~0;
									//tmpChar = getc(newFile);
									break;
								}
							default:
								{
									tmpStr = addChar2Str(tmpStr, tmpChar);
									//tmpChar = getc(newFile);
									break;
								}
							}
							tmpChar = getc(newFile);
						}
						break;
					}
					printInput((int**)inputData);
					tmpChar = getc(newFile);
					printf("\n|%c|\n", tmpChar);
				}
				break;
			}

/*		case CHAR_STR:
			{
				while ((tmpChar != EOF) && (tmpChar != '@'))
				{
					if (tmpChar == '{')
					{
						while (tmpChar != '}')
						{
							addChar2Str(typeStr, tmpChar);
							tmpChar = getc(newFile);
						}
						break;
					}
				}


				break;
			}
			*/
		case UNDEF_STR:
			{
				printf("\n ERROR! Unknown format.\n");
				break;
			}
		}
	}
	printf("\n Done.\n");
	return inputData;
}


