#include "ChallengeRunner.h"
#include "ChallengePt1.h"
#include "ChallengePt2.h"

int main()
{
	ChallengeRunner runner;
	runner.addChallenge(std::make_unique<ChallengePt1>());
	runner.addChallenge(std::make_unique<ChallengePt2>());
	runner.run();
	return 0;
}