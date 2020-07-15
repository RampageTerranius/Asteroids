#include "Entity.h"
#include "GameEngine.h"
#include "Debug.h"
#include "Random.h"
#include "Misc Functions.h"
#include <math.h>

Bullets allBullets = Bullets();
Asteroids allAsteroids = Asteroids();

void Entity::Draw()
{
	if (this->tex != nullptr)
		this->tex->Draw(game.GetRenderer().renderer, this->rotation, static_cast <int> (round(this->x)), static_cast <int> (round(this->y)));
}

bool Entity::PointIntersectsTexture(SDL_Point point)
{
	if (!this->tex->HasTexture())
		return false;

	SDL_Rect rect = this->tex->Rect();

	if (this->tex->centerTextureOnDraw)
	{
		rect.x = this->x - (rect.w / 2);
		rect.y = this->y - (rect.h / 2);

		if (SDL_PointInRect(&point, &rect))
			return true;
	}
	else
	{
		rect.x = this->x;
		rect.y = this->y;

		if (SDL_PointInRect(&point, &rect))
			return true;
	}

	return false;
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
		this->x = static_cast <float> (game.SCREEN_WIDTH - 1) + this->x;
	else if (this->x > game.SCREEN_WIDTH)
		this->x = 0 + (this->x - game.SCREEN_WIDTH);

	if (this->y < 0)
		this->y = static_cast <float> (game.SCREEN_HEIGHT - 1) + this->y;
	else if (this->y > game.SCREEN_HEIGHT)
		this->y = 0 + (this->y - game.SCREEN_HEIGHT);

	for (auto var : allAsteroids.allAsteroids)	
		if (GetDistance(this->x, var->x, this->y, var->y) <= (var->size / 2))
		{

			debug.Log("Bullet", "Update", "Bullet collided with asteroid");
			return false;
		}

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

void Asteroid::Break()
{

}

void Asteroids::CreateAsteroid(int x, int y, float velX, float velY, int size)
{
	Asteroid* asteroid = new Asteroid();

	int randomNum;

	Random random;

	asteroid->velX = velX;
	asteroid->velY = velY;

	asteroid->size = size;
	asteroid->tex = game.State()->allTextures.GetTexture("asteroid " + std::to_string(asteroid->size));

	asteroid->y = y;
	asteroid->x = x;

	allAsteroids.push_back(asteroid);

	debug.Log("Asteroids", "CreateAsteroid", "Created asteroid at " + std::to_string(asteroid->x) + " \\" + std::to_string(asteroid->y) + " with velocity " + std::to_string(asteroid->velX) + "\\" + std::to_string(asteroid->velY) + " at size of " + std::to_string(asteroid->size));
}

void Asteroids::CreateAsteroid()
{
	int randomNum;

	Random random;

	int x = 0;
	int y = 0;
	float velX = random.RandomFloat(-game.MAX_ASTEROID_VEL, game.MAX_ASTEROID_VEL);
	float velY = random.RandomFloat(-game.MAX_ASTEROID_VEL, game.MAX_ASTEROID_VEL);

	// Setup the asteroids size.
	// Asteroid sizes are in multiples of 5.
	int size = 5 * (random.RandomInt(0, 4) + 1);

	randomNum = random.RandomInt(0, 4);
	switch (randomNum)
	{
	case 0: // spawn randomly at left of screen.
		y = random.RandomInt(0, game.SCREEN_HEIGHT);
		break;

	case 1:// spawn randomly at right of screen.
		y = random.RandomInt(0, game.SCREEN_HEIGHT);
		x = game.SCREEN_WIDTH - 1;
		break;

	case 2:// spawn randomly at top of screen;
		x = random.RandomInt(0, game.SCREEN_WIDTH);
		break;

	case 3:// spawn randomly at bottom of screen;
		x = random.RandomInt(0, game.SCREEN_WIDTH);
		y = game.SCREEN_HEIGHT - 1;
		break;
	}

	CreateAsteroid(x, y, velX, velY, size);
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