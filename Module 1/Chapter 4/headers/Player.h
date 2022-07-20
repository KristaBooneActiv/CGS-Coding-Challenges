#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player(float aHealth, int x, int y);

	virtual void Draw() const override;
	void UseHealth(float aHealth);
	void PickupKey();

protected:
	bool  mHasKey;
};