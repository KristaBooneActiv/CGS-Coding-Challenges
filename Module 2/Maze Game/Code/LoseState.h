#pragma once
#include "IGameState.h"

class StateMachineExampleGame;

class LoseState : public IGameState
{
	StateMachineExampleGame* m_pOwner;

public:
	LoseState(StateMachineExampleGame* pOwner);
	~LoseState() = default;

	virtual bool Update() override;
	virtual void Draw() override;
};


