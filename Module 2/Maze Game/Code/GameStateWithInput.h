#pragma once
#include "IGameState.h"
#include "UserInputManager.h"

class GameStateWithInput : public IGameState
{
public:
	GameStateWithInput() = default;
	~GameStateWithInput() = default;

	virtual void Enter() override
	{
		m_userInputMgr.start();
	}

	virtual void Exit() override
	{
		m_userInputMgr.end();
	}

protected:
	UserInputManager m_userInputMgr;
};