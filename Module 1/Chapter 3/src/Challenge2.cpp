#include "Challenge2.h"
#include "Utilities.h"
#include <iostream>

void Challenge2::computeFactorsOf(size_t aNumber, std::vector<size_t>& aFactors) const
{
	for (size_t n = 1; n <= aNumber; ++n)
	{
		if (aNumber % n == 0)
		{
			aFactors.push_back(n);
		}
	}
}

std::string Challenge2::details() const
{
	return "Display all factors of a number provided by the user";
}

void Challenge2::run()
{
	// Prompt user for input
	std::cout << "Hello, there! Enter a positive number and I'll give you all the factors of it: ";
	std::optional<size_t> number = util::getNumberFromUser<size_t>();

	while (!number.has_value() || number.value() < 0)
	{
		std::cout << "That's not a positive number! Try again: ";
		number = util::getNumberFromUser<size_t>();
	}

	// Print feedback/result
	std::cout << "\nYou gave me: " << number.value() << ". Let me crunch some numbers..." << std::endl;

	std::vector<size_t> factors;
	computeFactorsOf(number.value(), factors);

	std::cout << "The factors of " << number.value() << " are: { ";
	util::printVector(std::cout, factors);
	std::cout << "}\n" << std::endl;
}