#pragma once

class Character
{
public:
	Character(float aHealth, int aX, int aY);

	virtual void Move(int aDeltaX, int aDeltaY);
	virtual void TakeDamage(float aDamage);
	virtual void Draw() const = 0;

protected:
	float mHealth;
	int   mXPosition;
	int   mYPosition;
	bool  mIsAlive {true};
};