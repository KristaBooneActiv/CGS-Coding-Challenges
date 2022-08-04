#include "SettingsState.h"

#include <iostream>
#include <conio.h>

#include "StateMachineExampleGame.h"
#include "AudioManager.h"

using namespace std;

constexpr int kEscapeKey = 27;
constexpr char kSound = '1';
constexpr char kMainMenu = '2';

SettingsState::SettingsState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
	, m_shouldDraw(true)
{
	m_userInputMgr.addInputCallback(inputKey::ONE, std::bind(&SettingsState::toggleSound, this));
	{
		auto fp = std::bind(&SettingsState::quitToMain, this);
		m_userInputMgr.addInputCallback(inputKey::TWO, fp);
		m_userInputMgr.addInputCallback(inputKey::ESCAPE, fp);
	}
}

bool SettingsState::Update()
{
	return false;
}

void SettingsState::Draw()
{
	if (m_shouldDraw)
	{
		system("cls");
		cout << endl << endl << endl;
		cout << "          - - - Settings - - -" << endl << endl;
		cout << "             " << kSound << ". Toggle Sound: ";
		if (AudioManager::GetInstance()->IsSoundOn())
		{
			cout << "ON" << endl;
		}
		else
		{
			cout << "OFF" << endl;
		}
		cout << "             " << kMainMenu << ". Back to Main Menu " << endl;
		m_shouldDraw = false;
	}	
}

void SettingsState::toggleSound()
{
	AudioManager::GetInstance()->ToggleSound();
	if (AudioManager::GetInstance()->IsSoundOn())
	{
		AudioManager::GetInstance()->PlayMoneySound();
	}
	m_shouldDraw = true;
}

void SettingsState::quitToMain()
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
	m_userInputMgr.interrupt();
}