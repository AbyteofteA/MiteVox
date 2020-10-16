
namespace aimods
{

	class Feedforward_NN
	{
	public:

		unsigned int fileSize;

		FormalNeuron*** layerArray;
		FormalNeuron** outputLayer;
		unsigned int amountOfLayers;

		unsigned int* amountOfLayerNeurons;

		float** errorArray = NULL;

		// Buffer pack.
		float* firstErrors = NULL;
		float** dE_dz = NULL;
		float*** errors = NULL;

		Feedforward_NN()
		{
			printf("\n [ Creating a new network ]\n");

			this->fileSize = sizeof(unsigned int);

			this->layerArray = NULL;
			this->outputLayer = NULL;
			this->amountOfLayers = 0;
			this->amountOfLayerNeurons = NULL;
			this->errorArray = NULL;

			this->firstErrors = NULL;
			this->dE_dz = NULL;
			this->errors = NULL;
		}

		void resizeNetwork(unsigned int size)
		{
			this->amountOfLayerNeurons = (unsigned int*)realloc(this->amountOfLayerNeurons,
				sizeof(unsigned int) * size);
			this->layerArray = (FormalNeuron***)realloc(this->layerArray, sizeof(FormalNeuron**) * size);

			this->fileSize += sizeof(unsigned int);
		}

		float** createNeuronBuffer()
		{
			float** err = (float**)malloc(sizeof(float*) * amountOfLayers);

			for (unsigned int i = 0; i < amountOfLayers; i++)
				err[i] = (float*)calloc(amountOfLayerNeurons[i], sizeof(float));

			return err;
		}

		float*** createAxonBuffer()
		{
			float*** err = (float***)malloc(sizeof(float**) * amountOfLayers);

			for (unsigned int l = 0; l < amountOfLayers; l++)
			{
				err[l] = (float**)malloc(sizeof(float*) * amountOfLayerNeurons[l]);

				for (unsigned int n = 0; n < amountOfLayerNeurons[l]; n++)
				{
					err[l][n] = (float*)calloc(layerArray[l][n]->amountOfAxons, sizeof(float));
				}
			}

			return err;
		}


		void initBufferPack_BackProp()
		{
			firstErrors = (float*)calloc(amountOfLayerNeurons[amountOfLayers - 1], sizeof(float));
			dE_dz = createNeuronBuffer();
			errors = createAxonBuffer();
		}

		void deleteBufferPack_BackProp()
		{
			free(firstErrors);
			for (unsigned int l = 0; l < amountOfLayers; l++)
			{
				if (dE_dz != NULL)
					free(dE_dz[l]);
				if (errors != NULL)
				{
					for (unsigned int n = 0; n < amountOfLayerNeurons[l]; n++)
					{
						free(errors[l][n]);
					}
					free(errors[l]);
				}
			}
			free(dE_dz);
			free(errors);

			this->firstErrors = NULL;
			this->dE_dz = NULL;
			this->errors = NULL;
		}

		void initializeLayer(unsigned int layerIndex)
		{
			//printf("\n Initializing a layer..\n");
			unsigned int i = 0;

			while (i < this->amountOfLayerNeurons[layerIndex])
			{
				unsigned int j = 0;

				while (j < this->layerArray[layerIndex][i]->amountOfAxons)
				{
					if (this->layerArray[0] != this->layerArray[layerIndex])
						this->layerArray[layerIndex][i]->axonArray[j] = (this->layerArray[this->amountOfLayers - 2])[j];
					j++;
				}
				i++;
			}
		}

		FormalNeuron** addLayer(unsigned int size, unsigned char function)
		{
			// Check if the size is correct.
			if (size <= 0)	
				return NULL;

			deleteBufferPack_BackProp();

			this->amountOfLayers++;
			printf("\n\tAdding %d neurons to the [%d] layer..\n", size, this->amountOfLayers);

			FormalNeuron** newLayer = (FormalNeuron**)malloc(sizeof(FormalNeuron*) * size);
			resizeNetwork(this->amountOfLayers);

			// Add a cell with the quantity of neurons in the new layer.
			this->amountOfLayerNeurons[this->amountOfLayers - 1] = size;

			// Add a cell with the pointers to layers.
			this->layerArray[this->amountOfLayers - 1] = newLayer;
			this->outputLayer = newLayer;

			// Allocation of array of pointers to neurons.
			for (unsigned int n = 0; n < size; n++)
			{
				if (this->layerArray[0] == newLayer)
				{
					this->layerArray[this->amountOfLayers - 1][n] = new FormalNeuron(function, 1);
					this->fileSize += sizeof(unsigned int) + sizeof(unsigned char) + sizeof(float) * 3;
				}
				else
				{
					this->layerArray[this->amountOfLayers - 1][n] = new FormalNeuron(function, this->amountOfLayerNeurons[this->amountOfLayers - 2]);
					this->fileSize += sizeof(unsigned int) + sizeof(unsigned char) + sizeof(float) * (2 + this->amountOfLayerNeurons[this->amountOfLayers - 2]);
				}
			}

			initializeLayer(this->amountOfLayers - 1);
			initBufferPack_BackProp();

			return newLayer;
		}

		/*****************************************************************************************
		Initializes outputs of neurons of the first layer with specified data.
		*****************************************************************************************/
		void inputData(float* data) // TO IMPROVE
		{
			if (this->amountOfLayers == NULL)
			{
				printf("\n ERROR! Network is empty.\n");
				return;
			}
			else if (this->amountOfLayers == 1)
			{
				printf("\n ERROR! Network is almost empty.\n");
				return;
			}

			//printf("\n Inputting data..\n");

			for (unsigned int i = 0; i < this->amountOfLayerNeurons[0]; i++)
				(this->layerArray[0])[i]->output = data[i];
		}

		/*****************************************************************************************
		Initializes outputs of neurons of the first layer with random data [-1; 1].
		*****************************************************************************************/
		void inputData()
		{
			if (this->amountOfLayers == NULL)
			{
				printf("\n ERROR! Network is empty.\n");
				return;
			}
			else if (this->amountOfLayers == 1)
			{
				printf("\n ERROR! Network is almost empty.\n");
				return;
			}

			for (unsigned int i = 0; i < this->amountOfLayerNeurons[0]; i++)
				(this->layerArray[0])[i]->output = (float)(rand() % 21 - 10) / 1000;
		}

		/*****************************************************************************************
		Propagates signals through the network.
		*****************************************************************************************/
		void propagate()
		{
			//printf("\n Propagating signals..\n");

			for (unsigned int i = 1; i < this->amountOfLayers; i++)
			{
				for (unsigned int j = 0; j < this->amountOfLayerNeurons[i]; j++)
					this->layerArray[i][j]->propagate();
			}
		}

		/*****************************************************************************************
		Returns index of a neuron with the maximum output.
		*****************************************************************************************/
		unsigned int netResponse()
		{
			//printf("\n Prepairing answer..\n");
			float max = 0;
			unsigned int maxIndex = 0;

			for (unsigned int i = 0; i < this->amountOfLayerNeurons[this->amountOfLayers - 1]; i++)	
			{
				if ((this->layerArray[this->amountOfLayers - 1])[i]->output > max)
				{
					max = (this->layerArray[this->amountOfLayers - 1])[i]->output;
					maxIndex = i;
				}
			}

			return maxIndex;
		}

		char* nameAFile();
		void writeTo_NNSH(char* filename);
		void readFrom_NNSH(char* filename);
		char* Feedforward_NN::writeTo_CHAR();
		void Feedforward_NN::readFrom_CHAR(char* bytes);

		float BackPropagation_STEP(LearningBatch* batch, float learningRate);
		float Validate(LearningBatch** testBatch, unsigned int batchSize);

		//	TO IMPLEMENT
		void deleteLayer(unsigned int index)
		{
			printf("\n Deleting a layer..\n");


		}
		//	TO IMPLEMENT
		void deleteNetwork()
		{
			printf("\n Deleting a network..\n");

			deleteBufferPack_BackProp();

			for (unsigned int l = 0; l < amountOfLayers; l++)
			{
				for (unsigned int n = 0; n < amountOfLayerNeurons[l]; n++)
				{
					delete layerArray[l][n];
				}
				free(layerArray[l]);
			}
			free(layerArray);
			free(amountOfLayerNeurons);

			this->fileSize = sizeof(unsigned int);

			this->layerArray = NULL;
			this->outputLayer = NULL;
			this->amountOfLayers = 0;
			this->amountOfLayerNeurons = NULL;
			this->errorArray = NULL;

			this->firstErrors = NULL;
			this->dE_dz = NULL;
			this->errors = NULL;
		}
		//	TO IMPLEMENT
		void joinNetworks(Feedforward_NN* secondNet)
		{
			printf("\n Joining the networks..\n");
			//Feedforward_NN* newNetwork = (Feedforward_NN*)malloc(sizeof(newNetwork));
		}
	};

}
