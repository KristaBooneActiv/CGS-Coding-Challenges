#pragma once
#include "PlacableActor.h"
class Key : public PlacableActor
{
public:
	Key(int x, int y, ActorColor color);
	virtual ActorType GetType() override { return ActorType::Key; }
};

