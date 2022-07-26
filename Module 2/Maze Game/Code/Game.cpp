#include "Game.h"

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
		// update with no input
		Update(false);
		// Draw
		Draw();
		// Update with input
		isGameOver = Update();
	}

	Draw();
}

void Game::Deinitialize()
{
	if (m_pStateMachine != nullptr)
		m_pStateMachine->Cleanup();
}

bool Game::Update(bool processInput)
{
	return m_pStateMachine->UpdateCurrentState(processInput);
}

void Game::Draw()
{
	m_pStateMachine->DrawCurrentState();
}
