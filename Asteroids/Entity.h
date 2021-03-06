#pragma once
#include <list>

#include "Textures.h"
#include "Audio.h"

class Entity
{
public:
	Entity();
	void Draw();
	virtual bool Update() = 0;

	Texture* tex;
	float x, y;
	float rotation;
};

class Player : public Entity
{
public:
	Player();
	bool Update();
	void FireWeapon();
	void Respawn();

	float velX, velY;
	float velocity;
	float turnRate;
	bool speedBoost;
	int fireTimer;
	int fireInterval;
	int immunityTime;

	Sound* moveForwardsSound;
	Sound* moveForwardsBoostSound;
	Sound* moveBackwardsSound;
	Sound* moveBackwardsBoostSound;
	Sound* equalizeSound;
};

class Bullet : public Entity
{
public:
	Bullet();
	bool Update();
	void Destroy();

	int distanceLeft;
	float velX, velY;
	float startX, startY;
	float speed;
};

class Bullets
{
public:
	void CreateBullet(Player* player, Texture* tex);
	void DestroyBullet(Bullet* bullet);
	void UpdateAll();
	void RenderAll();
	void Clear();

//private:
	std::list<Bullet*> allBullets;
};

extern Bullets allBullets;

class Asteroid : public Entity
{
public:
	Asteroid();
	bool Update();
	void Break(Bullet* bullet);
	float velX, velY;
	int size;
};

class Asteroids
{
public:
	void CreateAsteroid(Player* player);
	void CreateAsteroid(int x, int y, float velX, float velY, int size);
	void DestroyAsteroid(Asteroid* asteroid);
	void UpdateAll();
	void RenderAll();
	void Clear();

//private:
	std::list<Asteroid*> allAsteroids;
};

extern Asteroids allAsteroids;