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

}

void Bullet::Update()
{

}