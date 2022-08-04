#pragma once
#include "GameStateWithInput.h"
#include "Player.h"
#include "ShadowPlayer.h"
#include "Level.h"

#include "CollisionEngine.h"

#include <windows.h>
#include <vector>
#include <string>

#include "UserInputManager.h"

class StateMachineExampleGame;

class GameplayState : public GameStateWithInput
{
public:
	GameplayState(StateMachineExampleGame* pOwner);
	~GameplayState();

	virtual void Enter() override;
	virtual bool Update() override;
	virtual void Draw() override;

private:
	StateMachineExampleGame* m_pOwner;
	CollisionEngine m_collisionEngine;
	std::vector<std::string> m_LevelNames;

	Level* m_pLevel;
	Player m_player;
	ShadowPlayer* m_playerShadow;

	bool m_beatLevel;
	int m_currentLevel;

	int m_skipFrameCount;
	static constexpr int kFramesToSkip = 10;

	void HandleCollision(PlacableActor& aActor, int newX, int newY);
	bool Load();
	void DrawHUD(const HANDLE& console);

	// Functions called when input occurs
	void movePlayerUp();
	void movePlayerDown();
	void movePlayerLeft();
	void movePlayerRight();

	void movePlayer(int aX, int aY);

	void playerQuit();
	void playerDropKey();
};
