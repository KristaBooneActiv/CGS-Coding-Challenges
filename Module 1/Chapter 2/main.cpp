// Challenge 1:
// Write a program to find the “Sum” and “Average” of “three” numbers.
// Provide a hint to the user, take the input and return some output
// results to show the user the calculations for their three numbers.
//
// Challenge 2:
// Write a program that uses a function to return the value of a
// number cubed. Take the number as an input from the user and print
// the cubed value back to the user.

#include "Challenge1.h"
#include "Challenge2.h"
#include "ChallengeRunner.h"

#include <iostream>

int main()
{
	ChallengeRunner runner;
	runner.addChallenge(std::make_unique<Challenge1>(3));
	runner.addChallenge(std::make_unique<Challenge2>());
	runner.run();
}
