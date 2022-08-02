#pragma once
class GameplayState;

class PlacableActor;
class Player;
class Enemy;
class Money;
class Key;
class ShadowActivator;
class Door;
class Goal;

class CollisionEngine
{
public:
	// The result of the CollisionEngine will be something
	// that affects the game state.
	enum Result
	{
		ePlayerContinueWithMove,
		ePlayerContinueWithoutMove,
		eShadowActivated,
		ePlayerDeath,
		ePlayerHitGoal
	};

	Result Collide(Player& aPlayer, PlacableActor* aActorPtr2);

	// Overloaded for new types
	Result Collide(Player& aPlayer, Enemy& aEnemyPtr);
	Result Collide(Player& aPlayer, Money& aMoneyPtr);
	Result Collide(Player& aPlayer, Key& aKeyPtr);
	Result Collide(Player& aPlayer, ShadowActivator& aShadowActivatorPtr);
	Result Collide(Player& aPlayer, Door& aDoorPtr);
	Result Collide(Player& aPlayer, Goal& aGoalPtr);
};
