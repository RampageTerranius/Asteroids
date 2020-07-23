#include "Vector2D.h"

#include <string>

Vector2D::Vector2D(float newX, float newY)
{
	x = newX;
	y = newY;
}

void Vector2D::Add(float vec)
{
	x += vec;
	y += vec;
}

void Vector2D::Subtract(float vec)
{
	x -= vec;
	y -= vec;
}

void Vector2D::Multiply(float vec)
{
	x *= vec;
	y *= vec;
}

void Vector2D::Divide(float vec)
{
	x /= vec;
	y /= vec;
}

void Vector2D::Normalize()
{
	float len = Length();
	x /= len;
	y /= len;
};

float Vector2D::Length()
{
	return static_cast <float> (sqrt((x * x) + (y * y)));
};

std::string Vector2D::ToString()
{
	return std::to_string(x) + "/" + std::to_string(y);
}