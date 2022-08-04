#include "Game.h"
#include "AudioManager.h"

Game::Game()
	: m_pStateMachine(nullptr)
{

}

void Game::Initialize(GameStateMachine* pStateMachine)
{
	if (pStateMachine != nullptr)
	{
		pStateMachine->Init();
		m_pStateMachine = pStateMachine;
	}
}

void Game::RunGameLoop()
{
	bool isGameOver = false;
	while (!isGameOver)
	{
		// Update
		isGameOver = m_pStateMachine->UpdateCurrentState();

		// Draw
		m_pStateMachine->DrawCurrentState();
	}
}

void Game::Deinitialize()
{
	if (m_pStateMachine != nullptr)
		m_pStateMachine->Cleanup();
}
