#include "GameplayState.h"

#include <iostream>
#include <conio.h>
#include <assert.h>
#include <exception>

#include "Enemy.h"
#include "Key.h"
#include "Door.h"
#include "Money.h"
#include "Goal.h"
#include "AudioManager.h"
#include "Utility.h"
#include "StateMachineExampleGame.h"
#include "ShadowActivator.h"

using namespace std;

GameplayState::GameplayState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
	, m_beatLevel(false)
	, m_skipFrameCount(0)
	, m_currentLevel(0)
	, m_pLevel(nullptr)
	, m_playerShadow(nullptr)
	, m_collisionEngine()
{
	m_LevelNames.push_back("Level1.txt");
	m_LevelNames.push_back("Level2.txt");
	m_LevelNames.push_back("Level3.txt");

	// Set up user input manager callbacks
	{
		auto fp = std::bind(&GameplayState::movePlayerUp, this);
		m_userInputMgr.addInputCallback(inputKey::ARROW_UP, fp);
		m_userInputMgr.addInputCallback('W', fp);
		m_userInputMgr.addInputCallback('w', fp);
	}
	{
		auto fp = std::bind(&GameplayState::movePlayerDown, this);
		m_userInputMgr.addInputCallback(inputKey::ARROW_DOWN, fp);
		m_userInputMgr.addInputCallback('S', fp);
		m_userInputMgr.addInputCallback('s', fp);
	}
	{
		auto fp = std::bind(&GameplayState::movePlayerLeft, this);
		m_userInputMgr.addInputCallback(inputKey::ARROW_LEFT, fp);
		m_userInputMgr.addInputCallback('A', fp);
		m_userInputMgr.addInputCallback('a', fp);
	}
	{
		auto fp = std::bind(&GameplayState::movePlayerRight, this);
		m_userInputMgr.addInputCallback(inputKey::ARROW_RIGHT, fp);
		m_userInputMgr.addInputCallback('D', fp);
		m_userInputMgr.addInputCallback('d', fp);
	}
	{
		auto fp = std::bind(&GameplayState::playerDropKey, this);
		m_userInputMgr.addInputCallback('Z', fp);
		m_userInputMgr.addInputCallback('z', fp);
	}
	{
		auto fp = std::bind(&GameplayState::playerQuit, this);
		m_userInputMgr.addInputCallback('Q', fp);
		m_userInputMgr.addInputCallback('q', fp);
	}
	AudioManager::GetInstance()->PlayMainTheme();
}

GameplayState::~GameplayState()
{
	delete m_pLevel;
	m_pLevel = nullptr;

	if (m_playerShadow)
	{
		delete m_playerShadow;
		m_playerShadow = nullptr;
	}
}

bool GameplayState::Load()
{
	if (m_pLevel)
	{
		delete m_pLevel;
		m_pLevel = nullptr;
	}
	if (m_playerShadow)
	{
		delete m_playerShadow;
		m_playerShadow = nullptr;
	}

	m_pLevel = new Level();
	
	return m_pLevel->Load(m_LevelNames.at(m_currentLevel), m_player.GetXPositionPointer(), m_player.GetYPositionPointer());

}

void GameplayState::Enter()
{
	if (!Load())
	{
		throw std::runtime_error("FATAL ERROR: Unable to load level file");
	}
	GameStateWithInput::Enter();
}

bool GameplayState::Update()
{
	++m_skipFrameCount;
	if (m_skipFrameCount > kFramesToSkip)
	{
 		m_pLevel->UpdateActors(m_player.GetXPosition(), m_player.GetYPosition());
		m_skipFrameCount = 0;
	}

	if (m_beatLevel)
	{
		++m_currentLevel;
		if (m_currentLevel == m_LevelNames.size())
		{
			Utility::WriteHighScore(m_player.GetMoney());
			AudioManager::GetInstance()->PlayWinSound();

			m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Win);
		}
		else
		{
			m_beatLevel = false;

			// On to the next level
			Load();
		}
	}

	return false;
}

// =========================================================================
// Refactored this method to make use of the collision engine
// which handles managing placableActor behaviors when the collide.
// 
// Mostly removes the responsibility of managing collisions from the gameplay
// state. I considered having each object know how it interacts with other
// objects, but decided to have a 3rd party object to avoid adding too much
// info into the player/enemy/etc. classes.
// 
// There could be less code here if we passed a reference of GampeplayState
// to the CollisionEngine, and exposed some functions on GameplayState.
void GameplayState::HandleCollision(PlacableActor& aActor, int newX, int newY)
{
	PlacableActor* collidedActor = m_pLevel->UpdateActors(newX, newY);

	if (collidedActor && collidedActor->IsActive())
	{
		auto result = m_collisionEngine.Collide(m_player, collidedActor);
		switch (result)
		{
		case CollisionEngine::ePlayerContinueWithoutMove:
		{
			// no-op
			break;
		}
		case CollisionEngine::ePlayerContinueWithMove:
		{
			aActor.SetPosition(newX, newY);
			break;
		}		
		case CollisionEngine::eShadowActivated:
		{
			// Activate the shadow player
			// NOTE- not the best practice because we don't know where anything is!
			int shadowStartX = m_pLevel->GetWidth() - 2;
			int shadowStartY = m_pLevel->GetHeight() - 2;

			m_playerShadow = new ShadowPlayer(shadowStartX, shadowStartY);
			m_playerShadow->Place(shadowStartX, shadowStartY); // redundant, but required to get isActive=true...
			AudioManager::GetInstance()->PlayShadowActivatedSound();
			break;
		}
		case CollisionEngine::ePlayerDeath:
		{
			AudioManager::GetInstance()->PlayLoseSound();
			m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Lose);
			break;
		}
		case CollisionEngine::ePlayerHitGoal:
		{
			m_beatLevel = true;
			AudioManager::GetInstance()->PlayNextLevelSound();
			break;
		}
		}
	}
	else if (m_pLevel->IsSpace(newX, newY)) // no collision
	{
		aActor.SetPosition(newX, newY);
	}
	else if (m_pLevel->IsWall(newX, newY))
	{
		// wall collision, do nothing
	}
}

void GameplayState::Draw()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");

	m_pLevel->Draw();

	// Set cursor position for player 
	COORD actorCursorPosition;
	actorCursorPosition.X = m_player.GetXPosition();
	actorCursorPosition.Y = m_player.GetYPosition();
	SetConsoleCursorPosition(console, actorCursorPosition);
	m_player.Draw();
	
	if (m_playerShadow)
	{
		COORD actorCursorPosition;
		actorCursorPosition.X = m_playerShadow->GetXPosition();
		actorCursorPosition.Y = m_playerShadow->GetYPosition();
		SetConsoleCursorPosition(console, actorCursorPosition);
		m_playerShadow->Draw();
	}

	// Set the cursor to the end of the level
	COORD currentCursorPosition;
	currentCursorPosition.X = 0;
	currentCursorPosition.Y = m_pLevel->GetHeight();
	SetConsoleCursorPosition(console, currentCursorPosition);

	DrawHUD(console);
}

void GameplayState::DrawHUD(const HANDLE& console)
{
	cout << endl;

	// Top Border
	for (int i = 0; i < m_pLevel->GetWidth(); ++i)
	{
		cout << Level::WAL;
	}
	cout << endl;

	// Left Side border
	cout << Level::WAL;

	cout << " wasd-move " << Level::WAL << " z-drop key " << Level::WAL;

	cout << " $:" << m_player.GetMoney() << " " << Level::WAL;
	cout << " lives:" << m_player.GetLives() << " " << Level::WAL;
	cout << " key:";
	if (m_player.HasKey())
	{
		m_player.GetKey()->Draw();
	}
	else
	{
		cout << " ";
	}

	// RightSide border
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console, &csbi);

	COORD pos;
	pos.X = m_pLevel->GetWidth() - 1;
	pos.Y = csbi.dwCursorPosition.Y;
	SetConsoleCursorPosition(console, pos);

	cout << Level::WAL;
	cout << endl;

	// Bottom Border
	for (int i = 0; i < m_pLevel->GetWidth(); ++i)
	{
		cout << Level::WAL;
	}
	cout << endl;
}

void GameplayState::movePlayerUp()
{
	movePlayer(0, -1);
}

void GameplayState::movePlayerDown()
{
	movePlayer(0, 1);
}

void GameplayState::movePlayerLeft()
{
	movePlayer(-1, 0);
}

void GameplayState::movePlayerRight()
{
	movePlayer(1, 0);
}

void GameplayState::movePlayer(int aX, int aY)
{
	int newPlayerX = m_player.GetXPosition() + aX;
	int newPlayerY = m_player.GetYPosition() + aY;

	HandleCollision(m_player, newPlayerX, newPlayerY);

	// Move the shadow, if active and handle collisions
	if (m_playerShadow)
	{
		HandleCollision(*m_playerShadow,
			m_playerShadow->GetXPosition() + aX*-1,
			m_playerShadow->GetYPosition() + aY*-1);
	}
}

void GameplayState::playerQuit()
{
	m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
}

void GameplayState::playerDropKey()
{
	m_player.DropKey();
}