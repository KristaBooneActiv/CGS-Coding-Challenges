#include <iostream>
#include "Boat.h"

Boat::Boat(int aYears, int aMilesOrHours, const std::string& aBrand)
	: Vehicle(aYears, aMilesOrHours, aBrand)
{ /* no-op */ }

void Boat::Display()
{
	std::cout << "Here is a bit about my boat:" << std::endl;
	std::cout << "Hours: " << mMilesOrHours << std::endl;
	Vehicle::Display();
}

void Boat::Drive(int aHours)
{
	std::cout << "Driving my boat for " << aHours << " hour(s)!" << std::endl;
	Vehicle::Drive(aHours);
}