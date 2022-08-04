#include <iostream>
#include "MainMenuState.h"
#include "StateMachineExampleGame.h"

using namespace std;

MainMenuState::MainMenuState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
	, m_shouldQuit(false)
	, m_didDraw(false)
{
	m_userInputMgr.addInputCallback(inputKey::ONE, std::bind(&MainMenuState::LoadGameplay, this));
	m_userInputMgr.addInputCallback(inputKey::TWO, std::bind(&MainMenuState::LoadHighScores, this));
	m_userInputMgr.addInputCallback(inputKey::THREE, std::bind(&MainMenuState::LoadSettings, this));
	
	{
		auto fp = std::bind(&MainMenuState::SetQuit, this);
		m_userInputMgr.addInputCallback(inputKey::ESCAPE, fp);
		m_userInputMgr.addInputCallback(inputKey::FOUR, fp);
	}
}

bool MainMenuState::Update()
{
	return m_shouldQuit;
}

void MainMenuState::Draw()
{
	// We really only need to draw this once.
	if (!m_didDraw)
	{
		system("cls");
		cout << endl << endl << endl;
		cout << "          - - - MAIN MENU - - -" << endl << endl;
		cout << "             " << (char)inputKey::ONE << ". Play " << endl;
		cout << "             " << (char)inputKey::TWO << ". High Score " << endl;
		cout << "             " << (char)inputKey::THREE << ". Settings " << endl;
		cout << "             " << (char)inputKey::FOUR << ". Quit " << endl;
		m_didDraw = true;
	}
}

void MainMenuState::LoadGameplay()
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Gameplay);
	m_userInputMgr.interrupt();
}

void MainMenuState::LoadHighScores()
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::HighScore);
	m_userInputMgr.interrupt();
}

void MainMenuState::LoadSettings()
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Settings);
	m_userInputMgr.interrupt();
}

void MainMenuState::SetQuit()
{
	m_shouldQuit = true;
	m_userInputMgr.interrupt();
}