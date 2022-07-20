#pragma once
#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(float aHealth, int aX, int aY);

	virtual void Draw() const override;
};