#include <iostream>
#include "Player.h"

Player::Player(float aHealth, int aX, int aY)
	: Character(aHealth, aX, aY)
	, mHasKey(false)
{ /* no-op */ }

void Player::Draw() const
{
	std::cout << "@";
}

void Player::UseHealth(float aHealth)
{
	mHealth += aHealth;
	if (!mIsAlive)
	{
		mIsAlive = true;
	}
}

void Player::PickupKey()
{
	mHasKey = true;
}