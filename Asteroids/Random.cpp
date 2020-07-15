#include "Random.h"

float Random::RandomFloat(float min, float max)
{
	if (min >= max)
		return 0;

	this->Seed();

	return ( ( static_cast<float> (rand()) / static_cast<float> (RAND_MAX) ) * (max - min) ) + min;
}

int Random::RandomInt(int min, int max)
{
	if (min >= max)
		return 0;

	this->Seed();

	return rand() % (max - min) + min;
}

void Random::Seed()
{
	if (!this->seeded)
	{
		srand(static_cast <unsigned> (time(0)));
		seeded = true;
	}
}