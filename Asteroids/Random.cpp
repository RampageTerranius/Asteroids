#include "Random.h"

bool Random::seeded = false;

float Random::RandomFloat(float min, float max)
{
	if (min >= max)
		return 0;

	this->Seed();

	float f = ((static_cast<float> (rand()) / static_cast<float> (RAND_MAX)) * (max - min)) + min;

	return f;
}

int Random::RandomInt(int min, int max)
{
	if (min >= max)
		return 0;

	this->Seed();

	int i = rand() % (max - min + 1) + min;

	return i;
}

void Random::Seed()
{
	if (!this->seeded)
	{
		srand(static_cast <unsigned> (time(0)));
		seeded = true;
	}
}