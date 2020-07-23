#include "Vector2D.h"

#include <string>

Vector2D::Vector2D(float newX, float newY)
{
	this->x = newX;
	this->y = newY;
}

void Vector2D::Add(float vec)
{
	this->x += vec;
	this->y += vec;
}

void Vector2D::Subtract(float vec)
{
	this->x -= vec;
	this->y -= vec;
}

void Vector2D::Multiply(float vec)
{
	this->x *= vec;
	this->y *= vec;
}

void Vector2D::Divide(float vec)
{
	this->x /= vec;
	this->y /= vec;
}

void Vector2D::Normalize()
{
	float len = Length();
	this->x /= len;
	this->y /= len;
};

float Vector2D::Length()
{
	return static_cast <float> (sqrt((x * x) + (y * y)));
};

std::string Vector2D::ToString()
{
	return std::to_string(this->x) + "/" + std::to_string(this->y);
}