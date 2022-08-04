#pragma once
#include "IGameState.h"

class StateMachineExampleGame;

class WinState : public IGameState
{
	StateMachineExampleGame* m_pOwner;

public:
	WinState(StateMachineExampleGame* pOwner);
	~WinState() = default;

	virtual bool Update() override;
	virtual void Draw() override;
};
