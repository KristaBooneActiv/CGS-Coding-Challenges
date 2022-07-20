#include "ChallengeRunner.h"
#include "Challenge1.h"

int main()
{
	int arr[10] = {12, 899, 1, 0, -14, 77, 300, -123, 7, 2};

	ChallengeRunner runner;
	runner.addChallenge(std::make_unique<Challenge1<int, 10>>(arr));

	runner.run();
}
