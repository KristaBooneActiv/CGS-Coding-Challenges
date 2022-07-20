#include <ctime>
#include <iostream>

#include "Challenge1.h"

template<typename T, size_t N>
Challenge1::Challenge1(T(&& aArray)[N])
{

}

std::string Challenge1::details() const
{
	return "Checks and returns whether it is currently a leap year.";
}

void Challenge1::run()
{
	int year = getCurrentYear();
	bool isLeapYear = checkIsLeapYear(year);

	std::cout << "The current year is " << year << "." << std::endl;
	std::cout << "It is" << (isLeapYear ? "" : " not") << " a leap year!" << std::endl;
}