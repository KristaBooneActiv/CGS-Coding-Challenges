#include <iostream>
#include "Car.h"

Car::Car(int aYears, int aMiles, const std::string& aBrand)
	: mYear(aYears)
	, mMiles(aMiles)
	, mBrand(aBrand)
{ /* no-op */ }

void Car::Display()
{
	std::cout << "Brand: " << mBrand << std::endl;
	std::cout << "Miles: " << mMiles << std::endl;
	std::cout << "Year:  " << mYear << std::endl;
}

void Car::Drive(int aMiles)
{
	mMiles += aMiles;
}