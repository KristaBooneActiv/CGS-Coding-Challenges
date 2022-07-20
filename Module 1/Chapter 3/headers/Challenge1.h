// Challenge:
// Write a program to check and return whether it is currently a leap year.
// HINT - https://www.mathsisfun.com/leap-years.html
#pragma once
#include "IChallenge.h"

class Challenge1 : public IChallenge
{
public:
	std::string details() const override;
	void run() override;

protected:
	// Get the current year
	int getCurrentYear() const;

	// Check if a year is a leap year
	bool checkIsLeapYear(int aYear) const;
};