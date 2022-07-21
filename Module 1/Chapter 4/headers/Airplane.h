#pragma once
#include "Vehicle.h"
#include <string>

class Airplane : public Vehicle
{
public:
	Airplane() = default;
	Airplane(int aYear,
			int aMilesOrHours,
			const std::string& aBrand);
	~Airplane() = default;

	virtual void Display() override;
	virtual void Drive(int aHours) override;
};