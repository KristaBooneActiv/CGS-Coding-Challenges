// Challenge:
// Write a program to display all factors of a number provided
// by the user. Take an input of a positive valueand write back
// all of the values factors.
#pragma once
#include <vector>

#include "IChallenge.h"

class Challenge2 : public IChallenge
{
public:
	std::string details() const override;
	void run() override;

protected:
	// Compute the factors of a number
	void computeFactorsOf(size_t aNumber, std::vector<size_t>& aFactors) const;
};