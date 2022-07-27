#include "GameplayState.h"

#include <iostream>
#include <conio.h>
#include <windows.h>
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

constexpr int kArrowInput = 224;
constexpr int kLeftArrow = 75;
constexpr int kRightArrow = 77;
constexpr int kUpArrow = 72;
constexpr int kDownArrow = 80;
constexpr int kEscapeKey = 27;

GameplayState::GameplayState(StateMachineExampleGame* pOwner)
	: m_pOwner(pOwner)
	, m_beatLevel(false)
	, m_skipFrameCount(0)
	, m_currentLevel(0)
	, m_pLevel(nullptr)
	, m_playerShadow(nullptr)
{
	m_LevelNames.push_back("Level1.txt");
	m_LevelNames.push_back("Level2.txt");
	m_LevelNames.push_back("Level3.txt");
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
}

bool GameplayState::Update(bool processInput)
{
	if (processInput && !m_beatLevel)
	{
		int input = _getch();
		int arrowInput = 0;
		int newPlayerX = m_player.GetXPosition();
		int newPlayerY = m_player.GetYPosition();

		int shadowPlayerDeltaX = 0;
		int shadowPlayerDeltaY = 0;

		// One of the arrow keys were pressed
		if (input == kArrowInput)
		{
			arrowInput = _getch();
		}

		if ((input == kArrowInput && arrowInput == kLeftArrow) ||
			(char)input == 'A' || (char)input == 'a')
		{
			newPlayerX--;
			shadowPlayerDeltaX = 1;
		}
		else if ((input == kArrowInput && arrowInput == kRightArrow) ||
			(char)input == 'D' || (char)input == 'd')
		{
			newPlayerX++;
			shadowPlayerDeltaX = -1;
		}
		else if ((input == kArrowInput && arrowInput == kUpArrow) ||
			(char)input == 'W' || (char)input == 'w')
		{
			newPlayerY--;
			shadowPlayerDeltaY = 1;
		}
		else if ((input == kArrowInput && arrowInput == kDownArrow) ||
			(char)input == 'S' || (char)input == 's')
		{
			newPlayerY++;
			shadowPlayerDeltaY = -1;
		}
		else if (input == kEscapeKey)
		{
			m_pOwner->LoadScene(StateMachineExampleGame::SceneName::MainMenu);
		}
		else if ((char)input == 'Z' || (char)input == 'z')
		{
			m_player.DropKey();
		}

		// If position never changed
		if (newPlayerX == m_player.GetXPosition() && newPlayerY == m_player.GetYPosition())
		{
			//return false;
		}
		else
		{
			HandleCollision(m_player, newPlayerX, newPlayerY);

			// Move the shadow, if active and handle collisions
			if (m_playerShadow)
			{
				HandleCollision(*m_playerShadow,
					m_playerShadow->GetXPosition() + shadowPlayerDeltaX,
					m_playerShadow->GetYPosition() + shadowPlayerDeltaY);
			}
		}
	}
	if (m_beatLevel)
	{
		++m_skipFrameCount;
		if (m_skipFrameCount > kFramesToSkip)
		{
			m_beatLevel = false;
			m_skipFrameCount = 0;
			++m_currentLevel;
			if (m_currentLevel == m_LevelNames.size())
			{
				Utility::WriteHighScore(m_player.GetMoney());
				AudioManager::GetInstance()->PlayWinSound();
				
				m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Win);
			}
			else
			{
				// On to the next level
				Load();
			}

		}
	}

	return false;
}

void GameplayState::HandleCollision(PlacableActor& aActor, int newX, int newY)
{

	PlacableActor* collidedActor = m_pLevel->UpdateActors(newX, newY);
	if (collidedActor != nullptr && collidedActor->IsActive())
	{
		switch (collidedActor->GetType())
		{
		case ActorType::Enemy:
		{
			Enemy* collidedEnemy = dynamic_cast<Enemy*>(collidedActor);
			assert(collidedEnemy);
			AudioManager::GetInstance()->PlayLoseLivesSound();
			collidedEnemy->Remove();
			aActor.SetPosition(newX, newY);

			m_player.DecrementLives();
			if (m_player.GetLives() < 0)
			{
				//TODO: Go to game over screen
				AudioManager::GetInstance()->PlayLoseSound();
				m_pOwner->LoadScene(StateMachineExampleGame::SceneName::Lose);
			}
			break;
		}
		case ActorType::Money:
		{
			Money* collidedMoney = dynamic_cast<Money*>(collidedActor);
			assert(collidedMoney);
			AudioManager::GetInstance()->PlayMoneySound();
			collidedMoney->Remove();
			m_player.AddMoney(collidedMoney->GetWorth());
			aActor.SetPosition(newX, newY);
			break;
		}
		case ActorType::Key:
		{
			Key* collidedKey = dynamic_cast<Key*>(collidedActor);
			assert(collidedKey);
			if (!m_player.HasKey())
			{
				m_player.PickupKey(collidedKey);
				collidedKey->Remove();
				aActor.SetPosition(newX, newY);
				AudioManager::GetInstance()->PlayKeyPickupSound();
			}
			break;
		}
		case ActorType::ShadowActivator:
		{
			// NOTE- We should only ever get here if the actor passed into this funciton is a player!
			assert(aActor.GetType() == ActorType::Player);

			// No need to cast, just remove the shadow activator element.
			collidedActor->Remove();

			// Set the position of the player
			m_player.SetPosition(newX, newY);

			// Activate the shadow player
			// NOTE- not the best practice because we don't know where anything is!
			int shadowStartX = m_pLevel->GetWidth() - 2;
			int shadowStartY = m_pLevel->GetHeight() - 2;

			m_playerShadow = new ShadowPlayer(shadowStartX, shadowStartY);
			m_playerShadow->Place(shadowStartX, shadowStartY); // redundant, but required to get isActive=true...
			AudioManager::GetInstance()->PlayShadowActivatedSound();
			break;
		}
		case ActorType::Door:
		{
			Door* collidedDoor = dynamic_cast<Door*>(collidedActor);
			assert(collidedDoor);
			if (!collidedDoor->IsOpen())
			{
				if (m_player.HasKey(collidedDoor->GetColor()))
				{
					collidedDoor->Open();
					collidedDoor->Remove();
					m_player.UseKey();
					aActor.SetPosition(newX, newY);
					AudioManager::GetInstance()->PlayDoorOpenSound();
				}
				else
				{
					AudioManager::GetInstance()->PlayDoorClosedSound();
				}
			}
			else
			{
				aActor.SetPosition(newX, newY);
			}
			break;
		}
		case ActorType::Goal:
		{
			Goal* collidedGoal = dynamic_cast<Goal*>(collidedActor);
			assert(collidedGoal);
			collidedGoal->Remove();
			aActor.SetPosition(newX, newY);
			m_beatLevel = true;
			AudioManager::GetInstance()->PlayNextLevelSound();
			break;
		}
		default:
			break;
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