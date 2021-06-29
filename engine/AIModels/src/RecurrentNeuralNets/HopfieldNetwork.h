
#ifndef HOPFIELDNN_H
#define HOPFIELDNN_H

#include "engine/AIModels/src/Structure/FormalNeuron.h"

namespace aimods
{
	class HopfieldNetwork
	{
	public:

		float* output = nullptr;

		inline HopfieldNetwork(size_t _size);
		inline ~HopfieldNetwork();

		inline void inputData(float* data);
		inline void inputData();

		inline int isStable();
		inline void stabilize();
		inline void stabilizeInstantly();

		/// <summary>
		/// Propagates signals through the network one by one.
		/// </summary>
		inline void propagate();

		/// <summary>
		/// Propagates signals through the network all at the same time.
		/// </summary>
		inline void propagateInstantly();

		inline void learn();

		inline void print(size_t y = 1, size_t x = 0);

		// Getters //

		inline size_t getSize();

	private:

		FormalNeuron** neuronArray = nullptr;
		size_t size = 0;
		float* previousOutput = nullptr;

		inline void updateOutput();
	};


	// IMPLEMENTATION BELOW //


	inline HopfieldNetwork::HopfieldNetwork(size_t _size)
	{
		this->size = _size;
		this->neuronArray = new FormalNeuron*[this->size];
		this->previousOutput = new float[this->size];
		this->output = new float[this->size];

		for (size_t i = 0; i < this->size; i++)
		{
			this->neuronArray[i] = new FormalNeuron(ActivationFunction::BIPOLAR, this->size - 1);
		}
		for (size_t i = 0; i < this->size; i++)
		{
			size_t axonIndex = 0;
			for (size_t j = 0; j < this->size; j++)
			{
				if (i == j) continue;

				this->neuronArray[i]->axonArray[axonIndex] = this->neuronArray[j];
				axonIndex++;
			}
			this->neuronArray[i]->setWeights(0.0f);
		}
	}

	inline HopfieldNetwork::~HopfieldNetwork()
	{
		for (size_t i = 0; i < this->size; i++)
		{
			delete this->neuronArray[i];
		}

		delete[] this->neuronArray;
		delete[] this->previousOutput;
		delete[] this->output;
	}

	inline void HopfieldNetwork::inputData(float* data)
	{
		for (size_t i = 0; i < this->size; i++)
		{
			this->neuronArray[i]->output = data[i];
		}
	}

	inline void HopfieldNetwork::inputData()
	{
		for (size_t i = 0; i < this->size; i++)
		{
			this->neuronArray[i]->output = (float)(rand() % 21 - 10) / 1000;
		}
	}

	inline int HopfieldNetwork::isStable()
	{
		for (size_t i = 0; i < this->size; i++)
		{
			if ((int)previousOutput[i] != (int)output[i])
			{
				return 0;
			}
		}
		return 1;
	}

	inline void HopfieldNetwork::stabilize()
	{
		bool notStable = true;
		while (notStable)
		{
			this->propagate();
			notStable = !this->isStable();
		}
	}

	inline void HopfieldNetwork::stabilizeInstantly()
	{
		bool notStable = true;
		while (notStable)
		{
			this->propagateInstantly();
			notStable = !this->isStable();
		}
	}

	inline void HopfieldNetwork::propagate()
	{
		updateOutput();

		for (size_t i = 0; i < this->size; i++)
		{
			this->neuronArray[i]->propagate();
		}
	}

	inline void HopfieldNetwork::propagateInstantly()
	{
		updateOutput();

		for (size_t i = 0; i < this->size; i++)
		{
			this->output[i] = this->neuronArray[i]->computeOutput();
		}
		for (size_t i = 0; i < this->size; i++)
		{
			this->neuronArray[i]->output = this->output[i];
		}
	}

	inline void HopfieldNetwork::learn()
	{
		for (size_t i = 0; i < this->size; i++)
		{
			for (size_t j = 0; j < this->neuronArray[i]->getAmountOfAxons(); j++)
			{
				float deltaWeight = this->neuronArray[i]->output * this->neuronArray[i]->axonArray[j]->output;
				this->neuronArray[i]->weightArray[j] += deltaWeight;
			}
			this->neuronArray[i]->threshold -= this->neuronArray[i]->output;
		}
	}

	inline void HopfieldNetwork::print(size_t y, size_t x)
	{
		for (size_t i = 0; i < y; i++)
		{
			if (x == 0)
			{
				for (size_t j = 0; j < this->size; j++)
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
				for (size_t j = 0; j < x; j++)
				{
					size_t index = j + i * x;
					if (index >= this->size) return;

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

	inline size_t HopfieldNetwork::getSize()
	{
		return size;
	}

	inline void HopfieldNetwork::updateOutput()
	{
		for (size_t i = 0; i < this->size; i++)
		{
			previousOutput[i] = output[i];
		}
	}
}

#endif