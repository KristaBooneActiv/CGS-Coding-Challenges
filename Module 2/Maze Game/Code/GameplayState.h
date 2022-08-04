#pragma once
#include "GameState.h"
#include "Player.h"
#include "ShadowPlayer.h"
#include "Level.h"

#include "CollisionEngine.h"

#include <windows.h>
#include <vector>
#include <string>

#include "UserInputThread.h"

class StateMachineExampleGame;

class GameplayState : public GameState
{
public:
	GameplayState(StateMachineExampleGame* pOwner);
	~GameplayState();

	virtual void Enter() override;
	virtual bool Update(bool processInput = true) override;
	virtual void Draw() override;
	virtual void Exit() override;

	void ProcessInput(UserInputType aInput);

private:
	StateMachineExampleGame* m_pOwner;
	CollisionEngine m_collisionEngine;
	std::vector<std::string> m_LevelNames;

	Level* m_pLevel;
	Player m_player;
	ShadowPlayer* m_playerShadow;

	UserInputThread m_userInputThread;

	bool m_beatLevel;
	int m_currentLevel;

	int m_skipFrameCount;
	static constexpr int kFramesToSkip = 10;

	void HandleCollision(PlacableActor& aActor, int newX, int newY);
	bool Load();
	void DrawHUD(const HANDLE& console);
};
