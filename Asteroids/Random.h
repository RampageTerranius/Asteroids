#pragma once
#include <cstdlib>
#include <ctime>

class Random
{
public:
	float RandomFloat(float min, float max);
	int RandomInt(int min, int max);
private:
	void Seed();
	bool seeded;
};