#pragma once
#include "GameStateMachine.h"

class Game;
class IGameState;

class StateMachineExampleGame : public GameStateMachine
{
public:
	enum class SceneName
	{
		None,
		MainMenu,
		Gameplay,
		Settings,
		HighScore,
		Lose,
		Win,
	};

private:
	Game* m_pOwner;

	IGameState* m_pCurrentState;
	IGameState* m_pNextState;

public:
	StateMachineExampleGame(Game* pOwner);

	virtual bool Init() override;
	virtual bool UpdateCurrentState() override;
	virtual void DrawCurrentState() override;
	virtual void ChangeState(IGameState* pNewState) override;
	void LoadScene(SceneName scene);
	virtual bool Cleanup() override;
};

