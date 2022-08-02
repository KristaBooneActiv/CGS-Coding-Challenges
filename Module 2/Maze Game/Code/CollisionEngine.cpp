#include<sstream>
#include <stdexcept>

#include "AudioManager.h"
#include "CollisionEngine.h"
#include "GameplayState.h"
#include "PlacableActor.h"

#include "ShadowActivator.h"
#include "ShadowPlayer.h"
#include "Player.h"
#include "Enemy.h"
#include "Money.h"
#include "Door.h"
#include "Goal.h"
#include "Key.h"

// We still need some ugly type checking here...
// We could refactor this to use the visitor pattern, but I don't have enough time :)
CollisionEngine::Result CollisionEngine::Collide(Player& aPlayer, PlacableActor* aActorPtr2)
{
	switch (aActorPtr2->GetType())
	{
	case ActorType::Door:
	{
		return Collide(aPlayer, *(static_cast<Door*>(aActorPtr2)));
	}
	case ActorType::Enemy:
	{
		return Collide(aPlayer, *(static_cast<Enemy*>(aActorPtr2)));
	}
	case ActorType::Goal:
	{
		return Collide(aPlayer, *(static_cast<Goal*>(aActorPtr2)));
	}
	case ActorType::Key:
	{
		return Collide(aPlayer, *(static_cast<Key*>(aActorPtr2)));
	}
	case ActorType::Money:
	{
		return Collide(aPlayer, *(static_cast<Money*>(aActorPtr2)));
	}
	case ActorType::ShadowActivator:
	{
		return Collide(aPlayer, *(static_cast<ShadowActivator*>(aActorPtr2)));
	}
	default:
	{
		std::stringstream ss;
		ss << "Unrecognized placable actor type (" << ActorTypeToString(aActorPtr2->GetType())
			<< ") attempting to interact with player";
		throw std::runtime_error(ss.str());
	}
	}
}

CollisionEngine::Result CollisionEngine::Collide(Player& aPlayer, Enemy& aEnemyPtr)
{
	aEnemyPtr.Remove();

	aPlayer.DecrementLives();
	AudioManager::GetInstance()->PlayLoseLivesSound();

	return (aPlayer.GetLives() > 0) ? ePlayerContinueWithMove : ePlayerDeath;
}

CollisionEngine::Result CollisionEngine::Collide(Player& aPlayer, Money& aMoneyPtr)
{
	aMoneyPtr.Remove();
	aPlayer.AddMoney(aMoneyPtr.GetWorth());
	return ePlayerContinueWithMove;
}

CollisionEngine::Result CollisionEngine::Collide(Player& aPlayer, Key& aKeyPtr)
{
	if (aPlayer.HasKey())
	{
		return ePlayerContinueWithoutMove; // User must drop a key to pick this one up
	}

	aPlayer.PickupKey(&aKeyPtr);
	aKeyPtr.Remove();
	return ePlayerContinueWithMove;
}

CollisionEngine::Result CollisionEngine::Collide(Player& aPlayer, ShadowActivator& aShadowActivatorPtr)
{
	aShadowActivatorPtr.Remove();
	return eShadowActivated;
}

CollisionEngine::Result CollisionEngine::Collide(Player& aPlayer, Door& aDoorPtr)
{
	if (aDoorPtr.IsOpen())
	{
		return ePlayerContinueWithMove;
	}
	else if (aPlayer.HasKey(aDoorPtr.GetColor()))
	{
		aDoorPtr.Open();
		aDoorPtr.Remove();

		aPlayer.UseKey();
		AudioManager::GetInstance()->PlayDoorOpenSound();
		return ePlayerContinueWithMove;
	}
	else
	{
		AudioManager::GetInstance()->PlayDoorClosedSound();
		return ePlayerContinueWithoutMove;
	}
}

CollisionEngine::Result CollisionEngine::Collide(Player& aPlayer, Goal& aGoalPtr)
{
	aGoalPtr.Remove();
	return ePlayerHitGoal;
}