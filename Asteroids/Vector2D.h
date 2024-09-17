#pragma once

#include <string>

// class for platting a point in 2d space.
// Mainly used for calculating velocity for players/bullets/asteroids.
class Vector2D
{
public:
	Vector2D(float newX, float newY);

	void Add(float vec);
	void Subtract(float vec);
	void Multiply(float vec);
	void Divide(float vec);

	void Normalize();
	float Length();

	float x = 0.0f;
	float y = 0.0f;
};