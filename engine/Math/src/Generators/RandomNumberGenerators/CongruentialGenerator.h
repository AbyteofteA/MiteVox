
#ifndef CONGRUENTIALGENERATOR_H
#define CONGRUENTIALGENERATOR_H

namespace mathem
{
	class CongruentialGenerator
	{
	public:

		void seedRand(unsigned long _seed)
		{
			seed = _seed;
			tmpSeed = _seed;
		}

		unsigned long rand()
		{
			tmpSeed = tmpSeed * multiplier + increment;

			return tmpSeed;
		}

		float rand_float()
		{
			return (float)rand() / modulus;
		}

		double rand_double()
		{
			return (double)rand() / modulus;
		}

	private:

		unsigned long seed = 13;
		unsigned long multiplier = 3581641;
		unsigned long modulus = ~0;
		unsigned long increment = 13;

		unsigned long tmpSeed = 0;

	};
}

#endif