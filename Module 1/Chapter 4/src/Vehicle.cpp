#include <iostream>
#include "Vehicle.h"

Vehicle::Vehicle(int aYear, int aMilesOrHours, const std::string& aBrand)
	: mYear(aYear)
	, mMilesOrHours(aMilesOrHours)
	, mBrand(aBrand)
{ /* no-op */ }

void Vehicle::Display()
{
	std::cout << "Brand: " << mBrand << std::endl;
	std::cout << "Year:  " << mYear << std::endl;
}

void Vehicle::Drive(int aMilesOrHours)
{
	mMilesOrHours += aMilesOrHours;
}
