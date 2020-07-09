#include "Entity.h"
#include "GameEngine.h"

void Entity::Draw()
{
	this->tex->Draw(game.GetRenderer().renderer, this->x, this->y);
}

void Player::Update()
{

}

void Bullet::Update()
{

}