#pragma once
#include "GameStateWithInput.h"

class StateMachineExampleGame;

class LoseState : public GameStateWithInput
{
public:
	LoseState(StateMachineExampleGame* pOwner);
	~LoseState() = default;

	virtual bool Update() override;
	virtual void Draw() override;

private:
	StateMachineExampleGame* m_pOwner;
	bool m_doDraw;

	void QuitToMain();
};


