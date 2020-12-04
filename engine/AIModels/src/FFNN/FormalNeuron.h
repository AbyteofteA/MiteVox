

namespace aimods
{
	class FormalNeuron
	{
	public:

		unsigned char function = TanH;

		float output = 0;
		float threshold = 0;

		float* weightArray = NULL;
		FormalNeuron** axonArray = NULL;
		unsigned int amountOfAxons = 0;

		FormalNeuron(unsigned char _function = TanH, unsigned int _amountOfAxons = 1)
		{
			this->function = _function;
			formAxons(_amountOfAxons);
		}
		~FormalNeuron()
		{
			free(this->axonArray);
			free(this->weightArray);
		}

		void formAxons(unsigned int _amountOfAxons)
		{
			this->amountOfAxons = _amountOfAxons;

			this->axonArray = (FormalNeuron**)realloc(this->axonArray, sizeof(FormalNeuron*) * _amountOfAxons);
			this->weightArray = (float*)realloc(this->weightArray, sizeof(float) * _amountOfAxons);

			setWeights();
		}

		void setWeights(float value)
		{
			for (unsigned int a = 0; a < this->amountOfAxons; a++)
			{
				this->weightArray[a] = value;
			}
		}
		void setWeights()
		{
			for (unsigned int a = 0; a < this->amountOfAxons; a++)
			{
				this->weightArray[a] = (rand()%20 - 10) / (float)1000;
			}
		}

		float weightedSum()
		{
			float wSum = 0;
			for (unsigned int k = 0; k < this->amountOfAxons; k++)
				wSum += this->axonArray[k]->output * this->weightArray[k];
			wSum -= this->threshold;

			return wSum;
		}

		float computeOutput()
		{
			return activationFunction(weightedSum(), this->function);
		}
		float propagate()
		{
			this->output = activationFunction(weightedSum(), this->function);
			return this->output;
		}
	};
}