#pragma once
#include "IChallenge.h"

class Challenge2 : public IChallenge
{
public:
	std::string details() const override;
	void run() override;

protected:
	// Get a number from the user.
	bool getNumberFromUser();

	// Compute and return the cubed value of aNumber.
	double computeCube(double aNumber) const;

	// The number we will be cubing.
	double mNumber {0.};
};