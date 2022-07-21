#pragma once
#include "Vehicle.h"
#include <string>

class Car : public Vehicle
{
public:
	Car() = default;
	Car(int aYear,
		int aMilesOrHours,
		const std::string& aBrand);
	~Car() = default;

	virtual void Display() override;
	virtual void Drive(int aMiles) override;
};