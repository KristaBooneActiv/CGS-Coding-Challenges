#pragma once

class IGameState
{
public:
	virtual void Enter() {};
	virtual bool Update() = 0;
	virtual void Draw() = 0;
	virtual void Exit() {};
};