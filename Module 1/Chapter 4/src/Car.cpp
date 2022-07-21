#include <iostream>
#include "Car.h"

Car::Car(int aYears, int aMilesOrHours, const std::string& aBrand)
	: Vehicle(aYears, aMilesOrHours, aBrand)
{ /* no-op */ }

void Car::Display()
{
	std::cout << "Here is a bit about my car:" << std::endl;
	std::cout << "Miles: " << mMilesOrHours << std::endl;
	Vehicle::Display();
}

void Car::Drive(int aMiles)
{
	std::cout << "Driving my car " << aMiles << " miles!" << std::endl;
	Vehicle::Drive(aMiles);
}