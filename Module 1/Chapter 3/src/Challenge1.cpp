#include <ctime>
#include <iostream>

#include "Challenge1.h"

int Challenge1::getCurrentYear() const
{
	std::time_t t = std::time(nullptr);
	struct tm tInfo;
	localtime_s(&tInfo, &t);
	return 1900 + tInfo.tm_year;
}

bool Challenge1::checkIsLeapYear(int aYear) const
{
	bool divisible4 = (aYear % 4 == 0);
	bool divisible100 = (aYear % 100 == 0);
	bool divisible400 = (aYear % 400 == 0);

	return divisible4 && (!divisible100 || divisible400);
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