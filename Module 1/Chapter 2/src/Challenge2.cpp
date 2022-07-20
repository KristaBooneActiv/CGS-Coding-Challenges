#include "Challenge2.h"
#include "Utilities.h"

#include <iostream>

std::string Challenge2::details() const
{
    return "Computes the value of a number cubed";
}

bool Challenge2::getNumberFromUser()
{
	std::string userOption;
	std::cin >> userOption;

	if (util::isNumber(userOption))
	{
		mNumber = std::stod(userOption);
		return true;
	}

	return false;
}

double Challenge2::computeCube(double aNumber) const
{
	return std::pow(aNumber, 3);
}

void Challenge2::run() 
{
	// Prompt user for input
	std::cout << "Hello, there! Enter a number and I'll give you the value cubed: ";
	while (!getNumberFromUser())
	{
		std::cout << "That's not a number! Try again: ";
	}

	std::cout << "\nYou gave me: " << mNumber << ". Let me crunch some numbers..." << std::endl;
	std::cout << "The value cubed is: " << computeCube(mNumber) << "!" << std::endl;
}
