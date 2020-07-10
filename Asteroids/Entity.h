#pragma once
#include "Textures.h"

class Entity
{
public:
	void Draw();
	virtual void Update() = 0;

	Texture* tex;
	float x, y;
	float rotation;
	bool centerTexture = true;
};

class Player : public Entity
{
public:
	void Update();
	float velX, velY;
	float velocity;
	float turnRate;
	bool speedBoost;
	int fireTimer;
	int fireInterval;
};

class Bullet : public Entity
{
	void Update();
};

class Bullets
{

};