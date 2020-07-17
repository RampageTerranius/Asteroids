#pragma once
#include <list>

#include "Textures.h"

class Entity
{
public:
	void Draw();
	virtual bool Update() = 0;

	Texture* tex;
	float x, y;
	float rotation;
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
	void Destroy();

	float distanceLeft;
	float velX, velY;
	float speed;	
};

class Bullets
{
public:
	void CreateBullet(Player* player, Texture* tex);
	void DestroyBullet(Bullet* bullet);
	void UpdateAll();
	void RenderAll();

//private:
	std::list<Bullet*> allBullets;
};

extern Bullets allBullets;

class Asteroid : public Entity
{
public:
	bool Update();
	void Break();
	float velX, velY;
	int size;
};

class Asteroids
{
public:
	void CreateAsteroid();
	void CreateAsteroid(int x, int y, float velX, float velY, int size);
	void DestroyAsteroid(Asteroid* asteroid);
	void UpdateAll();
	void RenderAll();

//private:
	std::list<Asteroid*> allAsteroids;
};

extern Asteroids allAsteroids;