#pragma once
#include "GameStateWithInput.h"

#include <set>

class StateMachineExampleGame;

class HighScoreState : public GameStateWithInput
{
public:
	HighScoreState(StateMachineExampleGame* pOwner);
	~HighScoreState() = default;

	virtual bool Update() override;
	virtual void Draw() override;

private:
	StateMachineExampleGame* m_pOwner;
	std::set<int> m_HighScores;
	bool          m_doDraw;

	void QuitToMain();
};

