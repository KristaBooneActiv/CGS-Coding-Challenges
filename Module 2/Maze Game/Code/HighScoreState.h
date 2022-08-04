#pragma once
#include "IGameState.h"

#include <set>

class StateMachineExampleGame;

class HighScoreState : public IGameState
{
	StateMachineExampleGame* m_pOwner;
	std::set<int> m_HighScores;

public:
	HighScoreState(StateMachineExampleGame* pOwner);
	~HighScoreState() = default;

	virtual bool Update() override;
	virtual void Draw() override;
};

