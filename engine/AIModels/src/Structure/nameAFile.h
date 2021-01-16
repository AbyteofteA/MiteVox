
#ifndef NAMEAFILE_H
#define NAMEAFILE_H

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

#include <iostream>
#include <ctime>

namespace aimods
{

	/*****************************************************************************************
	Generates a name for a file based on date and time.
	*****************************************************************************************/
	char* nameAFile()
	{
		time_t t = time(nullptr);
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
}

#endif
