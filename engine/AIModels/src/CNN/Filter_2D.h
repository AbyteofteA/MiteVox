#pragma once


namespace aimods
{
	// Types of padding
#define VALID_PADDING		0
#define SAME_PADDING		1
#define MAX_POOLING			2

#define SQUASHED_OUTPUT		0
#define RGB_OUTPUT			1


	class Filter_2D
	{
	public:

		unsigned int size = 0;
		unsigned int stride = 1;
		unsigned char padding = SAME_PADDING;
		unsigned char squashChannels = 0;

		unsigned int amountOfChannels = 0;

		aimods::HyperData<float>* input = NULL;
		aimods::HyperData<float>* output = NULL;
		float** filters = NULL;

		Filter_2D(unsigned int _amountOfChannels, unsigned int _size, unsigned int _stride, unsigned char _padding)
		{
			tuneFilter(_amountOfChannels, _size, _stride, _padding);
		}

		void setFilter(float num)
		{
			for (unsigned int j = 0; j < size; j++)
			{
				for (unsigned int i = 0; i < size; i++)
				{
					for (unsigned int c = 0; c < amountOfChannels; c++)
					{
						filters[c][i + j * size] = num;
					}
				}
			}
		}

		void setFilter()
		{
			for (unsigned int j = 0; j < size; j++)
			{
				for (unsigned int i = 0; i < size; i++)
				{
					for (unsigned int c = 0; c < amountOfChannels; c++)
					{
						filters[c][i + j * size] = (float)(rand() % 200 - 100) / 50;
					}
				}
			}
		}

		/*****************************************************************************************
		Resizes filters and applies settings according to the parameters 
		*****************************************************************************************/
		void tuneFilter(unsigned int _amountOfChannels, unsigned int _size, unsigned int _stride, unsigned char _padding)
		{
			if (filters != NULL)
				deleteFilter();

			amountOfChannels = _amountOfChannels;
			size = _size;
			stride = _stride;
			padding = _padding;

			filters = (float**)realloc(filters, sizeof(float*) * amountOfChannels);
			for (unsigned int c = 0; c < amountOfChannels; c++)
			{
				filters[c] = (float*)malloc(sizeof(float) * size * size);
			}
			setFilter(0);
		}

		/*****************************************************************************************
		Resizes output and applies settings according to the settings
		*****************************************************************************************/
		void tuneOutput(unsigned char _squashChannels)
		{
			squashChannels = _squashChannels;

			unsigned int newX = 0;
			unsigned int newY = 0;
			if (padding == VALID_PADDING || padding == MAX_POOLING)
			{
				newX = (unsigned int)(floor(input->getDimention(2) - size) / stride + 1);
				newY = (unsigned int)(floor(input->getDimention(1) - size) / stride + 1);
			}
			else if (padding == SAME_PADDING)
			{
				newX = (unsigned int)(floor(input->getDimention(2) / stride));
				newY = (unsigned int)(floor(input->getDimention(1) / stride));
			}

			unsigned int* dims = (unsigned int*)malloc(sizeof(unsigned int) * 3);
			if (squashChannels == SQUASHED_OUTPUT)
				dims[0] = 1;
			else if (squashChannels == RGB_OUTPUT)
				dims[0] = input->getDimention(0);
			dims[1] = newY;
			dims[2] = newX;
			float* dat = (float*)malloc(sizeof(float) * dims[0] * newX * newY);

			if (output != NULL)
				delete output;
			output = new aimods::HyperData<float>(FLOAT_HYPERDATA, 3, dims, dat);
		}

		void attachInput(aimods::HyperData<float>* _input)
		{
			input = _input;
		}
		void attachOutput(aimods::HyperData<float>* _output)
		{
			output = _output;
		}
		aimods::HyperData<float>* getOutput()
		{
			return output;
		}

		void applyFilter()
		{
			unsigned int outputDimention_1 = output->getDimention(1);
			unsigned int outputDimention_2 = output->getDimention(2);

			for (unsigned int j = 0; j < outputDimention_1; j += 1)
			{
				for (unsigned int i = 0; i < outputDimention_2; i += 1)
				{
					if (padding == MAX_POOLING)
					{
						if (squashChannels == SQUASHED_OUTPUT)
						{
							output->data[i + j * outputDimention_2] = maxim(i * stride, j * stride, 0);
							if (amountOfChannels == 1) continue;

							for (unsigned int c = 1; c < amountOfChannels; c++)
							{
								if (maxim(i * stride, j * stride, c) > output->data[i + j * outputDimention_2])
									output->data[i + j * outputDimention_2] = maxim(i * stride, j * stride, c);
							}
						}
						else if (squashChannels == RGB_OUTPUT)
						{
							for (unsigned int c = 0; c < amountOfChannels; c++)
							{
								output->data[i + j * outputDimention_2 + c * outputDimention_1 * outputDimention_2]
									= maxim(i * stride, j * stride, c);
							}
						}
					}
					else
					{
						unsigned char cTmp = 0;
						output->data[i + j * outputDimention_2] = 0;

						for (unsigned int c = 0; c < amountOfChannels; c++)
						{
							if (squashChannels == SQUASHED_OUTPUT)
							{
								output->data[i + j * outputDimention_2 + cTmp * outputDimention_1 * outputDimention_2]
									+= weightedSum(i * stride, j * stride, c);
							}
							else if (squashChannels == RGB_OUTPUT)
							{
								output->data[i + j * outputDimention_2 + cTmp * outputDimention_1 * outputDimention_2]
									= weightedSum(i * stride, j * stride, c);
								cTmp++;
							}
						}
						//output->data[i + j * outputDimention_2] /= amountOfChannels;
					}
				}
			}

		}

		float weightedSum(unsigned int x, unsigned int y, unsigned int channel)
		{
			float result = 0;
			float filterSum = 0;
			unsigned int offsetX = 0;
			unsigned int offsetY = 0;
			if (padding == VALID_PADDING)
			{
				offsetX = 0;
				offsetY = 0;
			}
			else if (padding == SAME_PADDING)
			{
				offsetX = (unsigned int)(floor(size / 2));
				offsetY = (unsigned int)(floor(size / 2));
			}

			float pixelInput = 0;
			float pixelFilter = 0;
			unsigned int inputDimention_2 = input->getDimention(1);
			unsigned int inputDimention_1 = input->getDimention(2);

			for (unsigned int j = 0; j < size; j++)
			{
				for (unsigned int i = 0; i < size; i++)
				{
					unsigned int indx_X = x - offsetX + i;
					unsigned int indx_Y = y - offsetY + j;
					if (indx_X < 0 || indx_Y < 0)
						pixelInput = 0;
					else if (indx_X >= inputDimention_1 || indx_Y >= inputDimention_2)
						pixelInput = 0;
					else
						pixelInput = input->data[indx_X + indx_Y * inputDimention_1 +
						channel * inputDimention_1 * inputDimention_2];
					pixelFilter = filters[channel][i + j * size];
					filterSum += pixelFilter;

					result += pixelInput * pixelFilter;
				}
			}

			if (filterSum != 0)
				return result / filterSum;
			else
				return result;
		}

		float maxim(unsigned int x, unsigned int y, unsigned int channel)
		{
			float result = -2e9;
			float tmpResult = 0;

			unsigned int offsetX = 0;
			unsigned int offsetY = 0;

			unsigned int inputDimention_2 = input->getDimention(1);
			unsigned int inputDimention_1 = input->getDimention(2);

			for (unsigned int j = 0; j < size; j++)
			{
				for (unsigned int i = 0; i < size; i++)
				{
					unsigned int indx_X = x + i;
					unsigned int indx_Y = y + j;
					if (indx_X < 0 || indx_Y < 0)
						tmpResult = 0;
					else if (indx_X >= inputDimention_1 || indx_Y >= inputDimention_2)
						tmpResult = 0;
					else
						tmpResult = input->data[indx_X + indx_Y * inputDimention_1 +
						channel * inputDimention_1 * inputDimention_2];

					if (tmpResult > result) result = tmpResult;
				}
			}

			return result;
		}

		void deleteFilter()
		{
			for (unsigned int c = 0; c < amountOfChannels; c++)
			{
				delete[] filters[c];
			}
			delete[] filters;
			filters = NULL;
		}

		~Filter_2D()
		{
			deleteFilter();
			delete input;
			delete output;
		}
	};

}