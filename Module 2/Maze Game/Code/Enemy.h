#pragma once
#include "PlacableActor.h"
class Enemy : public PlacableActor
{
public:
	Enemy(int x, int y, int deltaX = 0, int deltaY = 0);

	virtual ActorType GetType() override { return ActorType::Enemy; }
	virtual void Draw() override;
	virtual void Update() override;

protected:
	void InitDirection();

private:
	int m_movementInX;
	int m_movementInY;

	int m_currentMovementX;
	int m_currentMovementY;

	int m_directionX;
	int m_directionY;

	void UpdateDirection(int& current, int& direction, int& movement);

};

