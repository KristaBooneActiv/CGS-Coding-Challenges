#include <iostream>
#include <conio.h>

#include "HighScoreState.h"
#include "StateMachineExampleGame.h"
#include "Utility.h"

using namespace std;

HighScoreState::HighScoreState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
	, m_doDraw(true)
{
	m_HighScores = Utility::WriteHighScore(0);
	m_userInputMgr.addInputCallback(inputKey::ENTER, std::bind(&HighScoreState::QuitToMain, this));
}

bool HighScoreState::Update()
{
	return false;
}

void HighScoreState::Draw()
{
	if (m_doDraw)
	{
		system("cls");
		cout << endl << endl << endl;
		cout << "          - - - HIGH SCORES - - -" << endl << endl;

		for (auto i = m_HighScores.rbegin(); i != m_HighScores.rend(); ++i)
		{
			cout << "             " << *i << endl;
		}

		cout << endl << endl;
		cout << "             Press enter to go back to the main menu" << endl << endl << endl;
		m_doDraw = false;
	}
}

void HighScoreState::QuitToMain()
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
	m_userInputMgr.interrupt();
}