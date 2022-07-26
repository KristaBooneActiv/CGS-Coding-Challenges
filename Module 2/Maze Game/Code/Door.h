#pragma once
#include "PlacableActor.h"

class Door : public PlacableActor
{
public:
	Door(int x, int y, ActorColor color, ActorColor closedColor);
	virtual void Draw() override;

	virtual ActorType GetType() override { return ActorType::Door;  }
	bool IsOpen() { return m_isOpen;  }
	void Open() { m_isOpen = true; }

private:
	bool m_isOpen;
	ActorColor m_closedColor;
};

