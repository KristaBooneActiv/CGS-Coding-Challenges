#pragma once
#include "PlacableActor.h"

class ShadowPlayer : public PlacableActor
{
public:
	ShadowPlayer(int x, int y);

	virtual ActorType GetType() override;
};