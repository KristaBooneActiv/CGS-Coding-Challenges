#include "LoseState.h"

#include <iostream>
#include <conio.h>

#include "StateMachineExampleGame.h"

using namespace std;


LoseState::LoseState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
	, m_doDraw(true)
{
	m_userInputMgr.addInputCallback(inputKey::ENTER, std::bind(&LoseState::QuitToMain, this));
}

bool LoseState::Update()
{
	return false;
}

void LoseState::Draw()
{
	if (m_doDraw)
	{
		system("cls");
		cout << endl << endl << endl;
		cout << "          - - - GAME OVER - - -" << endl << endl;
		cout << "             Better luck next time." << endl << endl << endl;
		cout << "             Press enter to go back to the main menu" << endl << endl << endl;
		m_doDraw;
	}
}

void LoseState::QuitToMain()
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
	m_userInputMgr.interrupt();
}
