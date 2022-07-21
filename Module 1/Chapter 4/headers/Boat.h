#pragma once
#include "Vehicle.h"
#include <string>

class Boat : public Vehicle
{
public:
	Boat() = default;
	Boat(int aYear,
		int aMilesOrHours,
		const std::string& aBrand);
	~Boat() = default;

	virtual void Display() override;
	virtual void Drive(int aHours) override;
};