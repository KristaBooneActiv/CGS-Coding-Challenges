#pragma once
#include <optional>

class NumberGuessingGame
{
public:
	NumberGuessingGame();
	NumberGuessingGame(int aNumber);
	NumberGuessingGame(int aRangeStart,
		               int aRangeEnd);
	~NumberGuessingGame() = default;

	struct GuessResponse
	{
		bool guessWasRight{ false };
		std::optional<bool> shouldGuessHigher{};
	};
	GuessResponse IsNumber(int aNumber);
	bool ShouldGiveHint() const;
	bool IsGameOver() const;

	int GetMaxGuesses() const;
	int GetHowManyGuessesUntilHintsActivated() const;

protected:
	int mNumber;
	int mNumGuesses{ 0 };
	int mMaxGuesses{ 10 };
	bool mGameOverFlag{ false };


	static int GenerateRandomNumber(int aRangeStart,
		                            int aRangeEnd);
};