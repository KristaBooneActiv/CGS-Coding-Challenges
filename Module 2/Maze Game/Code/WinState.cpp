#include "WinState.h"

#include <iostream>
#include <conio.h>

#include "StateMachineExampleGame.h"

using namespace std;

WinState::WinState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
	, m_doDraw(true)
{
	m_userInputMgr.addInputCallback(inputKey::ENTER, std::bind(&WinState::QuitToMain, this));
}

bool WinState::Update()
{
	return false;
}

void WinState::Draw()
{
	if (m_doDraw)
	{
		system("cls");
		cout << endl << endl << endl;
		cout << "          - - - WELL DONE - - -" << endl << endl;
		cout << "             You beat the game!" << endl << endl << endl;
		cout << "             Press enter to go back to the main menu" << endl << endl << endl;
		m_doDraw = false;
	}	
}

void WinState::QuitToMain()
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
	m_userInputMgr.interrupt();
}