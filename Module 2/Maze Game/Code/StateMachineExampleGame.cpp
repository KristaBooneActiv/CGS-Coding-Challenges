#include "StateMachineExampleGame.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "SettingsState.h"
#include "HighScoreState.h"
#include "LoseState.h"
#include "WinState.h"
#include "Game.h"

StateMachineExampleGame::StateMachineExampleGame(Game* pOwner)
	: m_pOwner(pOwner)
	, m_pCurrentState(nullptr)
	, m_pNextState(nullptr)
{
}

bool StateMachineExampleGame::Init()
{
	LoadScene(SceneName::MainMenu);
	return true;
}

bool StateMachineExampleGame::UpdateCurrentState(bool processInput)
{
	bool done = false;
	if (m_pNextState != nullptr)
	{
		ChangeState(m_pNextState);
		m_pNextState = nullptr;
	}

	if (m_pCurrentState != nullptr)
	{
		done = m_pCurrentState->Update(processInput);
	}
	return done;
}

void StateMachineExampleGame::DrawCurrentState()
{
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Draw();
	}
}

void StateMachineExampleGame::ChangeState(GameState* pNewState)
{
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Exit();
	}

	delete m_pCurrentState;
	m_pCurrentState = pNewState;
	pNewState->Enter();
}

void StateMachineExampleGame::LoadScene(SceneName scene)
{
	switch (scene)
	{
	case SceneName::None:
		// do nothing
		break;
	case SceneName::MainMenu:
		m_pNextState = new MainMenuState(this);
		break;
	case SceneName::Gameplay:
		m_pNextState = new GameplayState(this);
		break;
	case SceneName::Settings:
		m_pNextState = new SettingsState(this);
		break;
	case SceneName::HighScore:
		m_pNextState = new HighScoreState(this);
		break;
	case SceneName::Win:
		m_pNextState = new WinState(this);
		break;
	case SceneName::Lose:
		m_pNextState = new LoseState(this);
		break;
	}

}

bool StateMachineExampleGame::Cleanup()
{
	if (m_pCurrentState != nullptr)
	{
		m_pCurrentState->Exit();
		delete m_pCurrentState;
	}

	return true;
}
