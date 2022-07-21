#include "ChallengeRunner.h"
#include "Challenge1.h"
#include "Challenge2.h"
#include "Challenge3.h"

int main()
{
	int arr[10] = {12, 899, 1, 0, -14, 77, 300, -123, 7, 2};

	ChallengeRunner runner;
	runner.addChallenge(std::make_unique<Challenge1<int, 10>>(arr));
	runner.addChallenge(std::make_unique<Challenge2>());
	runner.addChallenge(std::make_unique<Challenge3>());
	runner.run();
}
