#pragma once
#include "IChallenge.h"
#include <ostream>
#include <vector>

class Challenge1 : public IChallenge
{
public:
	Challenge1(int numNumbersToUse);
	std::string details() const override;
	void run() override;

protected:
	// Compute and return the sum of aNumbers (up to numNumbers).
	double computeSum(const std::vector<double>& aNumbers) const;

	// Compute and return the average of aNumbers (up to numNumbers).
	double computeAverage(const std::vector<double>& aNumbers) const;

	// Parse standard input for numbers separated by spaces.
	void parseCin(std::vector<double>& aNumbers) const;

	// Print a vector of numbers (up to numNumbers) to an output stream.
	void printNumbers(std::ostream& aOut, const std::vector<double>& aNumbers) const;

	// Number of numbers to use to compute sum/average
	int mNumNumbers;
};
