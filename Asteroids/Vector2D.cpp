#include "Vector2D.h"

#include <string>

// Constructor, setup the base vector.
Vector2D::Vector2D(float newX, float newY)
{
	x = newX;
	y = newY;
}

// Add to the point.
void Vector2D::Add(float vec)
{
	x += vec;
	y += vec;
}

// Subtract from the point.
void Vector2D::Subtract(float vec)
{
	x -= vec;
	y -= vec;
}

// Multiply the point by the given number.
void Vector2D::Multiply(float vec)
{
	x *= vec;
	y *= vec;
}

// Divide the 2d point by the given number.
void Vector2D::Divide(float vec)
{
	x /= vec;
	y /= vec;
}

// Normalize the vector.
void Vector2D::Normalize()
{
	float len = Length();
	x /= len;
	y /= len;
};

// Calculate the magnitute of the vector using the Pythagorean theorem.
float Vector2D::Length()
{
	return static_cast <float> (sqrt((x * x) + (y * y)));
};