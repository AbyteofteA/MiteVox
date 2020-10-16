

namespace aimods
{
	class FormalNeuron
	{
	public:

		unsigned char function;

		float output;
		float threshold;

		float* weightArray;
		FormalNeuron** axonArray;
		unsigned int amountOfAxons;

		FormalNeuron()
		{
			this->function = TanH;
			this->output = 0;
			this->threshold = 1;
			this->amountOfAxons = 0;

			this->weightArray = NULL;
			this->axonArray = NULL;
		}
		FormalNeuron(unsigned char _function, unsigned int _amountOfAxons)
		{
			this->function = _function;
			this->output = 0;
			this->threshold = 1;
			this->amountOfAxons = _amountOfAxons;

			this->weightArray = NULL;
			this->axonArray = NULL;

			this->weightArray = (float*)malloc(sizeof(float) * _amountOfAxons);
			for (unsigned int a = 0; a < this->amountOfAxons; a++)
			{
				this->weightArray[a] = (float)(rand() % 201 - 100) / 1000;
			}
			this->axonArray = (FormalNeuron**)malloc(sizeof(FormalNeuron*) * _amountOfAxons);
		}

		float weightedSum()
		{
			float wSum = 0;
			for (unsigned int k = 0; k < this->amountOfAxons; k++)
				wSum += this->axonArray[k]->output * this->weightArray[k];
			wSum -= this->threshold;

			return wSum;
		}

		float propagate()
		{
			this->output = activationFunction(weightedSum(), this->function);
			return this->output;
		}

		void deleteFormalNeuron()
		{
			free(this->axonArray);
			free(this->weightArray);
		}
	};
}