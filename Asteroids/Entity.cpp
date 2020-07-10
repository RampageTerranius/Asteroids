#include "Entity.h"
#include "GameEngine.h"
#include <math.h>

void Entity::Draw()
{
	if (this->tex != nullptr)
		this->tex->Draw(game.GetRenderer().renderer, this->rotation, static_cast <int> (round(this->x)), static_cast <int> (round(this->y)));
}

void Player::Update()
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
}

void Bullet::Update()
{

}