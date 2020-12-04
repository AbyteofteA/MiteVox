#pragma once


namespace aimods
{

	class Hopfield_NN
	{
	public:

		FormalNeuron** neuronArray = NULL;
		unsigned int amountOfNeurons = 0;
		float* previousOutput = NULL;
		float* output = NULL;

		Hopfield_NN(unsigned int size)
		{
			init(size);
		}
		~Hopfield_NN()
		{
			wipe();
		}

		void init(unsigned int size)
		{
			this->neuronArray = (FormalNeuron**)malloc(sizeof(FormalNeuron*) * size);
			this->previousOutput = (float*)malloc(sizeof(float) * size);
			this->output = (float*)malloc(sizeof(float) * size);
			amountOfNeurons = size;

			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
			{
				this->neuronArray[i] = new FormalNeuron(BIPOLAR, size - 1);
			}
			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
			{
				unsigned int axonIndex = 0;
				for (unsigned int j = 0; j < this->amountOfNeurons; j++)
				{
					if (i == j) continue;

					this->neuronArray[i]->axonArray[axonIndex] = this->neuronArray[j];
					axonIndex++;
				}
				this->neuronArray[i]->setWeights(0.0f);
			}
		}
		void wipe()
		{
			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
			{
				delete this->neuronArray[i];
			}
			free(this->neuronArray);
			free(this->previousOutput);
			free(this->output);
			amountOfNeurons = 0;
		}
		void reset(unsigned int size)
		{
			wipe();
			init(size);
		}

		void inputData(float* data)
		{
			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
				this->neuronArray[i]->output = data[i];
		}
		void inputData()
		{
			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
				this->neuronArray[i]->output = (float)(rand() % 21 - 10) / 1000;
		}

		void updateOutput()
		{
			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
			{
				previousOutput[i] = output[i];
			}
		}
		int isStable()
		{
			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
			{
				if ((int)previousOutput[i] != (int)output[i])
				{
					return 0;
				}
			}
			return 1;
		}
		void stabilize()
		{
			bool notStable = true;
			while (notStable)
			{
				this->propagate();
				notStable = !this->isStable();
			}
		}

		/*****************************************************************************************
		Propagates signals through the network.
		*****************************************************************************************/
		void propagate()
		{
			updateOutput();

			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
			{
				this->neuronArray[i]->propagate();
			}
		}
		void propagate_instant()
		{
			updateOutput();

			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
			{
				this->output[i] = this->neuronArray[i]->computeOutput();
			}
			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
			{
				this->neuronArray[i]->output = this->output[i];
			}
		}

		void learn()
		{
			for (unsigned int i = 0; i < this->amountOfNeurons; i++)
			{
				for (unsigned int j = 0; j < this->neuronArray[i]->amountOfAxons; j++)
				{
					float deltaWeight = this->neuronArray[i]->output * this->neuronArray[i]->axonArray[j]->output;
					this->neuronArray[i]->weightArray[j] += deltaWeight;
				}
				this->neuronArray[i]->threshold -= this->neuronArray[i]->output;
			}
		}

		void print(unsigned int y = 1, unsigned int x = 0)
		{
			for (unsigned int i = 0; i < y; i++)
			{
				if (x == 0)
				{
					for (unsigned int j = 0; j < this->amountOfNeurons; j++)
					{
						if ((int)this->neuronArray[j]->output > 0)
						{
							printf("#");
						}
						else
						{
							printf("_");
						}
					}
				}
				else
				{
					for (unsigned int j = 0; j < x; j++)
					{
						unsigned int index = j + i * x;
						if (index >= this->amountOfNeurons) return;

						if ((int)this->neuronArray[j]->output > 0)
						{
							printf("#");
						}
						else
						{
							printf("_");
						}
					}
				}
				printf("\n");
			}
		}

	};
}
