#include "ChallengeRunner.h"
#include "Challenge1.h"
#include "Challenge2.h"

int main()
{
	ChallengeRunner runner;
	runner.addChallenge(std::make_unique<Challenge1>());
	runner.addChallenge(std::make_unique<Challenge2>());
	runner.run();
}