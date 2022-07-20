#include <iostream>
#include "Enemy.h"

Enemy::Enemy(float aHealth, int aX, int aY)
	: Character(aHealth, aX, aY)
{ /* no-op */ }

void Enemy::Draw() const
{
	std::cout << "*";
}