#include "Entity.h"
#include "GameEngine.h"
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
	if (this->velX > game.MAX_VEL)
		this->velX = game.MAX_VEL;
	if (this->velX < -game.MAX_VEL)
		this->velX = -game.MAX_VEL;

	if (this->velY > game.MAX_VEL)
		this->velY = game.MAX_VEL;
	if (this->velY < -game.MAX_VEL)
		this->velY = -game.MAX_VEL;

	this->x += this->velX;
	this->y += this->velY;

	if (this->x < 0)
		this->x = 0;
	else if (this->x > game.SCREEN_WIDTH)
		this->x = static_cast <float> (game.SCREEN_WIDTH);

	if (this->y < 0)
		this->y = 0;
	else if (this->y > game.SCREEN_HEIGHT)
		this->y = static_cast <float> (game.SCREEN_HEIGHT);

	// Reset the speed boost back to false, otherwise the palyer will permanently be boosted.
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

	if (this->distanceLeft < 0)
		return false;

	return true;
}

void Bullets::CreateBullet(Player* player, Texture* tex)
{
	Bullet* bullet = new Bullet();

	bullet->x = player->x;
	bullet->y = player->y;
	bullet->velX = cos((player->rotation + 90.0f) * (M_PI / 180)) * (player->velocity + 1);
	bullet->velY = sin((player->rotation + 90.0f) * (M_PI / 180)) * (player->velocity + 1);
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
	return true;
}

void Asteroids::UpdateAll()
{
}

void Asteroids::RenderAll()
{
}