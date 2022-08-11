#include <random>

#include "NumberGuessingGame.h"

constexpr int cGUESSES_UNTIL_HINT_ACTIVATED = 3;

NumberGuessingGame::NumberGuessingGame()
	: mNumber(GenerateRandomNumber(mRangeMin, mRangeMax))
{ /* no-op */ }

NumberGuessingGame::NumberGuessingGame(int aNumber)
	: mNumber(aNumber)
{ /* no-op */ }

NumberGuessingGame::NumberGuessingGame(int aRangeStart, int aRangeEnd)
	: mRangeMin(aRangeStart)
	, mRangeMax(aRangeEnd)
	, mNumber(GenerateRandomNumber(mRangeMin, mRangeMax))
{ /* no-op */ }

NumberGuessingGame::GuessResponse NumberGuessingGame::IsNumber(int aNumber)
{
	mNumGuesses++;

	GuessResponse response;
	response.guessWasRight = (aNumber == mNumber);
	mGameOverFlag = response.guessWasRight || (mNumGuesses == mMaxGuesses);
		
	if (!mGameOverFlag && ShouldGiveHint())
	{
		response.shouldGuessHigher = (aNumber < mNumber);
	}
	return response;
}

bool NumberGuessingGame::IsGameOver() const
{
	return mGameOverFlag;
}

void NumberGuessingGame::Restart()
{
	mNumber = GenerateRandomNumber(mRangeMin, mRangeMax);
	mGameOverFlag = false;
	mNumGuesses = 0;
}

bool NumberGuessingGame::ShouldGiveHint() const
{
	return mNumGuesses >= cGUESSES_UNTIL_HINT_ACTIVATED; // If player has gotten 3 incorrect guesses, give them a hint
}

int NumberGuessingGame::GetMaxGuesses() const
{
	return mMaxGuesses;
}

int NumberGuessingGame::GetHowManyGuessesUntilHintsActivated() const
{
	return cGUESSES_UNTIL_HINT_ACTIVATED;
}

int NumberGuessingGame::GenerateRandomNumber(int aRangeStart, int aRangeEnd)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(aRangeStart, aRangeEnd);
	return dist6(rng);
}