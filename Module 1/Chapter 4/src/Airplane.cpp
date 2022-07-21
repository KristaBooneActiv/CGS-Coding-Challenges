#include <iostream>
#include "Airplane.h"

Airplane::Airplane(int aYears, int aMilesOrHours, const std::string& aBrand)
	: Vehicle(aYears, aMilesOrHours, aBrand)
{ /* no-op */ }

void Airplane::Display()
{
	std::cout << "Here is a bit about my airplane:" << std::endl;
	std::cout << "Hours: " << mMilesOrHours << std::endl;
	Vehicle::Display();
}

void Airplane::Drive(int aHours)
{
	std::cout << "Flying my airplane for " << aHours << " hour(s)!" << std::endl;
	Vehicle::Drive(aHours);
}