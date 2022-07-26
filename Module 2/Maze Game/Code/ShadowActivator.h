#pragma once
#include "PlacableActor.h"

class ShadowActivator : public PlacableActor
{
public:
	ShadowActivator(int x, int y);
	~ShadowActivator() = default;

	virtual ActorType GetType() override { return ActorType::ShadowActivator; }
};