#include "Entity.h"
#include "GameEngine.h"
#include "Debug.h"
#include "Random.h"
#include <math.h>

Bullets allBullets = Bullets();
Asteroids allAsteroids = Asteroids();

void Entity::Draw()
{
	if (this->tex != nullptr)
		this->tex->Draw(game.GetRenderer().renderer, this->rotation, static_cast <int> (round(this->x)), static_cast <int> (round(this->y)));
}

bool Player::Update()
{
	// Make sure player hasnt gone over max velocity.
	if (this->velX > game.MAX_VEL)
		this->velX = game.MAX_VEL;
	if (this->velX < -game.MAX_VEL)
		this->velX = -game.MAX_VEL;

	if (this->velY > game.MAX_VEL)
		this->velY = game.MAX_VEL;
	if (this->velY < -game.MAX_VEL)
		this->velY = -game.MAX_VEL;

	// Add the velocity to its respective axis.
	this->x += this->velX;
	this->y += this->velY;

	if (this->x < 0)
		this->x = static_cast <float> (game.SCREEN_WIDTH - 1);
	else if (this->x > game.SCREEN_WIDTH)
		this->x = 0;

	if (this->y < 0)
		this->y = static_cast <float> (game.SCREEN_HEIGHT - 1);
	else if (this->y > game.SCREEN_HEIGHT)
		this->y = 0;

	// Reset the speed boost back to false, otherwise the player will permanently be boosted.
	this->speedBoost = false;

	if (this->fireTimer > 0)
		this->fireTimer--;

	return true;
}

void Player::FireWeapon()
{
	if (this->fireTimer <= 0)
	{
		this->fireTimer = this->fireInterval;
		allBullets.CreateBullet(this, game.State()->allTextures.GetTexture("bullet"));
	}
}

bool Bullet::Update()
{
	this->x -= this->velX;
	this->y -= this->velY;
	this->distanceLeft--;	

	if (this->x < 0)
		this->x = static_cast <float> (game.SCREEN_WIDTH - 1);
	else if (this->x > game.SCREEN_WIDTH)
		this->x = 0;

	if (this->y < 0)
		this->y = static_cast <float> (game.SCREEN_HEIGHT - 1);
	else if (this->y > game.SCREEN_HEIGHT)
		this->y = 0;

	if (this->distanceLeft < 0)
		return false;

	return true;
}

void Bullets::CreateBullet(Player* player, Texture* tex)
{
	Bullet* bullet = new Bullet();

	bullet->x = player->x;
	bullet->y = player->y;
	bullet->velX = cos((player->rotation + 90.0f) * (M_PI / 180)) * ((player->velX * 0.25) + game.BULLET_VELOCITY);
	bullet->velY = sin((player->rotation + 90.0f) * (M_PI / 180)) * ((player->velY * 0.25) + game.BULLET_VELOCITY);
	bullet->distanceLeft = game.BULLET_DISTANCE;
	bullet->tex = tex;

	this->allBullets.push_back(bullet);
}

void Bullets::UpdateAll()
{
	for (auto it = this->allBullets.begin(); it != this->allBullets.end();)
		if (!(*it)->Update())
		{
			delete (*it);
			it = this->allBullets.erase(it);
		}
		else
			++it;
}

void Bullets::RenderAll()
{
	for (auto& var : this->allBullets)
		var->Draw();
}

bool Asteroid::Update()
{
	this->x -= this->velX;
	this->y -= this->velY;

	if (this->x < 0)
		this->x = static_cast <float> (game.SCREEN_WIDTH - 1);
	else if (this->x > game.SCREEN_WIDTH)
		this->x = 0;

	if (this->y < 0)
		this->y = static_cast <float> (game.SCREEN_HEIGHT - 1);
	else if (this->y > game.SCREEN_HEIGHT)
		this->y = 0;

	return true;
}

void Asteroids::CreateAsteroid()
{
	Asteroid* asteroid = new Asteroid();

	int randomNum;

	Random random;

	asteroid->velX = random.RandomFloat(-game.MAX_ASTEROID_VEL, game.MAX_ASTEROID_VEL);
	asteroid->velY = random.RandomFloat(-game.MAX_ASTEROID_VEL, game.MAX_ASTEROID_VEL);

	// Setup the asteroids size.
	// Asteroid sizes are in multiples of 5.
	asteroid->size = 5 * (random.RandomInt(0, 4) + 1);
	asteroid->tex = game.State()->allTextures.GetTexture("asteroid " + std::to_string(asteroid->size));

	randomNum = random.RandomInt(0, 4);
	switch (randomNum)
	{
	case 0: // spawn randomly at left of screen.
		asteroid->y = random.RandomInt(0, game.SCREEN_HEIGHT);

		allAsteroids.push_back(asteroid);
		break;

	case 1:// spawn randomly at right of screen.
		asteroid->y = random.RandomInt(0, game.SCREEN_HEIGHT);
		asteroid->x = game.SCREEN_WIDTH - 1;		

		allAsteroids.push_back(asteroid);
		break;

	case 2:// spawn randomly at top of screen;
		asteroid->x = random.RandomInt(0, game.SCREEN_WIDTH);

		allAsteroids.push_back(asteroid);
		break;

	case 3:// spawn randomly at bottom of screen;
		asteroid->x = random.RandomInt(0, game.SCREEN_WIDTH);
		asteroid->y = game.SCREEN_HEIGHT - 1;		

		allAsteroids.push_back(asteroid);
		break;
	}	

	debug.Log("Asteroids", "CreateAsteroid", "Created asteroid at " + std::to_string(asteroid->x) + " \\" + std::to_string(asteroid->y) + " with velocity " + std::to_string(asteroid->velX) + "\\" + std::to_string(asteroid->velY) + " at size of " + std::to_string(asteroid->size));
}

void Asteroids::UpdateAll()
{
	for (auto& var : this->allAsteroids)
		var->Update();
}

void Asteroids::RenderAll()
{
	for (auto& var : this->allAsteroids)
		var->Draw();
}