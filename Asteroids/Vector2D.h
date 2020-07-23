#pragma once

#include <string>

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

	std::string ToString();

	float x = 0.0f;
	float y = 0.0f;
};