
#ifndef SERVICE_H
#define SERVICE_H

#include "AIModels/src/Structure/Functions.h"

#include <iostream>

/************************************************************************************
Add character to a string.
************************************************************************************/
char* addChar2Str(char* str, char ch)
{
	if (str == nullptr)
	{
		str = (char*)malloc(sizeof(char) * 2);
		str[0] = ch;
		str[1] = '\0';
	}
	else
	{
		unsigned int strSize = 0;
		while(str[strSize] != '\0')
			strSize++;
		strSize++;

		str = (char*)realloc(str, sizeof(char) * (strSize + 1));
		str[strSize - 1] = ch;
		str[strSize] = '\0';
	}
	return str;
}


void showTitle()
{
	printf("\n   ");
	for (int i = 0; i < 69; i++)
		printf("_");
	printf("\n");
	printf("  #  \t _   _ _   _   ____  _   _ ___ _     ____  _____ ____\t\t#\n");
	printf("  |  \t| \\ | | \\ | | | __ )| | | |_ _| |   |  _ \\| ____|  _ \\\t\t|\n");
	printf("  |  \t|  \\| |  \\| | |  _ \\| | | || || |   | | | |  _| | |_) |\t\t|\n");
	printf("  |  \t| |\\  | |\\  | | |_) | |_| || || |___| |_| | |___|  _ < \t\t|\n");
	printf("  |  \t|_| \\_|_| \\_| |____/ \\___/|___|_____|____/|_____|_| \\_\\\t\t|\n  #");
	for (int i = 0; i < 69; i++)
		printf("_");
	printf("#\n\n");
}

#endif