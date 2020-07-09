#pragma once
#include "Textures.h"

class Entity
{
public:
	void Draw();
	virtual void Update() = 0;

	Texture* tex;
	int x, y;
	float rotation;
};

class Player : public Entity
{
	void Update();
};

class Bullet : public Entity
{
	void Update();
};

class Bullets
{

};