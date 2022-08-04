#pragma once
#include "GameStateWithInput.h"
#include "UserInputManager.h"

class StateMachineExampleGame;

class MainMenuState : public GameStateWithInput
{
public:
	MainMenuState(StateMachineExampleGame* pOwner);
	~MainMenuState() = default;

	virtual bool Update() override;
	virtual void Draw() override;

private:
	StateMachineExampleGame* m_pOwner;
	bool                     m_shouldQuit;
	bool                     m_didDraw;

	void LoadGameplay();
	void LoadHighScores();
	void LoadSettings();
	void SetQuit();
};

