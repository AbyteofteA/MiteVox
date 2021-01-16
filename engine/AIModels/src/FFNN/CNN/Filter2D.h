
#ifndef FILTER_2D_H
#define FILTER_2D_H

#include "AIModels/src/Structure/HyperData.h"

namespace aimods
{
	enum struct padding_type { VALID = 0, SAME };

#define SQUASHED_OUTPUT		0
#define RGB_OUTPUT			1


	class Filter2D
	{
	public:

		unsigned int size = 0;
		unsigned int stride = 1;
		padding_type padding = padding_type::SAME;
		unsigned char squashChannels = 0;

		unsigned int amountOfChannels = 0;

		aimods::HyperData<float>* input = nullptr;
		aimods::HyperData<float>* output = nullptr;
		float** filters = nullptr;

		Filter2D(unsigned int _amountOfChannels, unsigned int _size, unsigned int _stride, padding_type _padding)
		{
			tuneFilter(_amountOfChannels, _size, _stride, _padding);
		}
		~Filter2D()
		{
			deleteFilter();
			delete input;
			delete output;
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
		void tuneFilter(unsigned int _amountOfChannels, unsigned int _size, unsigned int _stride, padding_type _padding)
		{
			if (filters != nullptr)
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
			if (padding == padding_type::VALID)
			{
				newX = (unsigned int)(floor(input->getDimention(2) - size) / stride + 1);
				newY = (unsigned int)(floor(input->getDimention(1) - size) / stride + 1);
			}
			else if (padding == padding_type::SAME)
			{
				newX = (unsigned int)(floor(input->getDimention(2) / stride));
				newY = (unsigned int)(floor(input->getDimention(1) / stride));
			}

			if (output != nullptr)
				delete output;
			float* dat = nullptr;
			output = new aimods::HyperData<float>(hyperdataType::FLOAT);
			if (squashChannels == SQUASHED_OUTPUT)
			{
				output->resize(3, 1, newY, newX);
				dat = (float*)malloc(sizeof(float) * newX * newY);
			}
			else if (squashChannels == RGB_OUTPUT)
			{
				output->resize(3, 3, newY, newX);
				dat = (float*)malloc(sizeof(float) * 3 * newX * newY);
			}
			output->data = dat;
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

		float weightedSum(unsigned int x, unsigned int y, unsigned int channel)
		{
			float result = 0;
			float filterSum = 0;
			unsigned int offsetX = 0;
			unsigned int offsetY = 0;
			if (padding == padding_type::VALID)
			{
				offsetX = 0;
				offsetY = 0;
			}
			else if (padding == padding_type::SAME)
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
			filters = nullptr;
		}

	};

}

#endif