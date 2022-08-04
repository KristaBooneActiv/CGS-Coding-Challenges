#pragma once
#include "GameStateWithInput.h"
#include "UserInputManager.h"

class StateMachineExampleGame;

class SettingsState : public GameStateWithInput
{
public:
	SettingsState(StateMachineExampleGame* pOwner);
	~SettingsState() = default;

	virtual bool Update() override;
	virtual void Draw() override;

private:
	StateMachineExampleGame* m_pOwner;
	bool                     m_shouldDraw;

	void toggleSound();
	void quitToMain();
};
