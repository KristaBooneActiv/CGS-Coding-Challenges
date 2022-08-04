#pragma once
#include "GameStateWithInput.h"

class StateMachineExampleGame;

class WinState : public GameStateWithInput
{
public:
	WinState(StateMachineExampleGame* pOwner);
	~WinState() = default;

	virtual bool Update() override;
	virtual void Draw() override;

private:
	StateMachineExampleGame* m_pOwner;
	bool m_doDraw;

	void QuitToMain();
};
