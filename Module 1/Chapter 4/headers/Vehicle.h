#pragma once
#include <string>

class Vehicle
{
public:
	Vehicle() = default;
	Vehicle(int aYear,
			int aMilesOrHours,
			const std::string& aBrand);
	~Vehicle() = default;

	virtual void Display();
	virtual void Drive(int aMilesOrHours);

protected:
	int mYear{ 0 };
	int mMilesOrHours{ 0 };
	std::string mBrand{ "Unspecified" };
};