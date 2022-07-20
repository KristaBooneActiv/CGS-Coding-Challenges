#include "Character.h"

Character::Character(float aHealth, int aX, int aY)
	: mHealth(aHealth)
	, mXPosition(aX)
	, mYPosition(aY)
{ /* no-op */}

void Character::Move(int aDeltaX, int aDeltaY)
{
	mXPosition += aDeltaX;
	mYPosition += aDeltaY;
}

void Character::TakeDamage(float aDamage)
{
	if (!mIsAlive)
	{
		return;
	}

	mHealth -= aDamage;
	if (mHealth <= 0)
	{
		mHealth = 0;
		mIsAlive = false;
	}
}