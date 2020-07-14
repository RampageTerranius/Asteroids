#pragma once
#include <list>

#include "Textures.h"

class Entity
{
public:
	void Draw();
	virtual bool Update() = 0;
	//bool PointIntersectsTexture(SDL_Point point);

	Texture* tex;
	float x, y;
	float rotation;
	bool centerTexture = true;
};

class Player : public Entity
{
public:
	bool Update();
	void FireWeapon();

	float velX, velY;
	float velocity;
	float turnRate;
	bool speedBoost;
	int fireTimer;
	int fireInterval;
};

class Bullet : public Entity
{
public:
	bool Update();

	float distanceLeft;
	float velX, velY;
	float speed;	
};

class Bullets
{
public:
	void CreateBullet(Player* player, Texture* tex);
	void UpdateAll();
	void RenderAll();

private:
	std::list<Bullet*> allBullets;
};

extern Bullets allBullets;

class Asteroid : public Entity
{
public:
	bool Update();
	float velX, velY;
	int size;
};

class Asteroids
{
public:
	void CreateAsteroid();
	void UpdateAll();
	void RenderAll();

private:
	std::list<Asteroid*> allAsteroids;
};

extern Asteroids allAsteroids;