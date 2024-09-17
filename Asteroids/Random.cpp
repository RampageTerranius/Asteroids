#include "Random.h"

#include <cstdlib>
#include <ctime>

bool Random::seeded = false;

// Get a random floating point value between min and max.
float Random::RandomFloat(float min, float max)
{
	if (min >= max)
	{
		return 0;
	}

	Seed();

	float f = ((static_cast<float> (rand()) / static_cast<float> (RAND_MAX)) * (max - min)) + min;

	return f;
}

// Get a random integer value between min and max.
int Random::RandomInt(int min, int max)
{
	if (min >= max)
	{
		return 0;
	}

	Seed();

	int i = rand() % (max - min + 1) + min;

	return i;
}

// Seed the random number generator
void Random::Seed()
{
	if (!seeded)
	{
		// Take the current time and use it for the seed.
		srand(static_cast <unsigned> (time(0)));
		seeded = true;
	}
}