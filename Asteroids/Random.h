#pragma once
#include <cstdlib>
#include <ctime>

// Please keep in mind this class has some slight bias towards lower values.
class Random
{
public:
	float RandomFloat(float min, float max);
	int RandomInt(int min, int max);
private:
	void Seed();
	bool seeded;
};