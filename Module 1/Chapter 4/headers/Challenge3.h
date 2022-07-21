//Challenges:
//
// Create a vehicle base class, and from that create the necessary
// classes to get to boat, carand airplane. All classes should have
// an overridable function named Drive. Create an object of each
// and demonstrate the Drive function works. Feel free to add any
// additional functionality to better simulate these vehicles!

#pragma once
#include "IChallenge.h"
#include "Vehicle.h"

class Challenge3 : public IChallenge
{
public:
	Challenge3();
	~Challenge3() = default;

	std::string details() const override;
	void run() override;

protected:
	void printVehicles();

	std::vector<std::unique_ptr<Vehicle>> mVehicles;
};