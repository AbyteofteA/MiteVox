

/************************************************************************************
Add character to a string.
************************************************************************************/
char* addChar2Str(char* str, char ch)
{
	if (str == NULL)
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

template<typename T>
char* writeBinaryToString(char* str, unsigned int* poiter, T data)
{
	char* tmp = (char*)&data;
	if (str != NULL)
	{
		for (int i = 0; i < sizeof(T); i++)
		{
			sprintf((str + (*poiter)), "%c", tmp[i]);
			(*poiter)++;
		}
	}
	return str;
}


int whichStr(char * str)
{
	if (str == NULL)
	{
		printf("\n ERROR! String is not found.\n");
		return UNDEF_STR;
	}

	char * intStr = "int";
	char * charStr = "char";
	int answer = UNDEF_STR;

	if (str[0] == intStr[0])
	{
		unsigned int i = 1;
		while(str[i] != '\0')
		{
			if (intStr[i] == '\0')
			{
				answer = UNDEF_STR;
				break;
			}
			else if (str[i] == intStr[i])
				answer = INT_STR;
			else
			{
				answer = UNDEF_STR;
				break;
			}
			i++;
		}
	}
	else if (str[0] == charStr[0])
	{
		unsigned int i = 1;
		while(str[i] != '\0')
		{
			if (charStr[i] == '\0')
			{
				answer = UNDEF_STR;
				break;
			}
			else if (str[i] == charStr[i])
				answer = CHAR_STR;
			else
			{
				answer = UNDEF_STR;
				break;
			}
			i++;
		}
	}

	return answer;
}


void printInput(int ** input)
{
	if (input == NULL)
	{
		printf("\n ERROR! Array is not found.\n");
		return;
	}

	printf("\n");
	unsigned int i = 0;
	while (input[i] != '\0')
	{
		printf("\t{ ");
		unsigned int j = 0;
		while ((input[i])[j] != ~0)
		{
			printf("%d, ", (input[i])[j]);
			j++;
		}
		printf("}\n");
		i++;
	}
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