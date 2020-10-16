
#ifndef CONVERTATIONS_H
#define CONVERTATIONS_H

namespace mathem
{
	struct CongruentialGenerator
	{
		unsigned long seed = 13;
		unsigned long multiplier = 3581641;
		unsigned long modulus = ~0;
		unsigned long increment = 13;

		unsigned long tmpSeed;

	} CongruentialGenerator;

	void seedRand(unsigned long _seed)
	{
		CongruentialGenerator.seed = _seed;
		CongruentialGenerator.tmpSeed = _seed;
	}

	unsigned long rand()
	{
		unsigned long result = (CongruentialGenerator.tmpSeed *
			CongruentialGenerator.multiplier +
			CongruentialGenerator.increment);

		CongruentialGenerator.tmpSeed = result;

		return result;
	}

	double rand_double()
	{
		return (double)rand() / CongruentialGenerator.modulus;
	}
}

#endif