#pragma once

class IGameState;

class GameStateMachine
{
public:
	virtual ~GameStateMachine() = default;

	virtual bool Init() = 0;
	virtual bool UpdateCurrentState() = 0;
	virtual void DrawCurrentState() = 0;
	virtual void ChangeState(IGameState* pNewState) = 0;
	virtual bool Cleanup() = 0;
};

