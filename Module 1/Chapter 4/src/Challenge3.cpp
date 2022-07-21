#include <iostream>
#include <memory>
#include <vector>

#include "Airplane.h"
#include "Boat.h"
#include "Car.h"
#include "Challenge3.h"

Challenge3::Challenge3()
{
	// Using vector to easily print info, but accessing using indices later.
	// Not best practice, but reasonable enough for this small project.
	// idx 0= airplane
	mVehicles.push_back(std::make_unique<Airplane>(2002, 50, "Commuter Craft"));
	// idx 1= boat
	mVehicles.push_back(std::make_unique<Boat>(1994, 1000, "Yamaha"));
	// idx 2= car
	mVehicles.push_back(std::make_unique<Car>(2014, 70000, "Chevy"));
}

void Challenge3::printVehicles()
{
	for (const auto& vehicle : mVehicles)
	{
		vehicle->Display();
	}
}

std::string Challenge3::details() const
{
	return "Create a vehicle, boat, car, and airplane class. Override \"Drive()\" function for each and demonstrate.";
}

void Challenge3::run()
{
	std::cout << "Here are my vehicles!" << std::endl;
	printVehicles();

	std::cout << "Let's take my airplane out for a spin..." << std::endl;
	mVehicles.at(0)->Drive(1);

	const int milesToLake = 20;
	std::cout << "Let's spend a day on the lake. Let's hop in the car to get there...(this Chevy can totally haul the boat)..." << std::endl;
	mVehicles.at(2)->Drive(milesToLake);

	std::cout << "Lake time..." << std::endl;
	mVehicles.at(1)->Drive(6);

	std::cout << "Time to head home..." << std::endl;
	mVehicles.at(2)->Drive(milesToLake);

	std::cout << "What a busy day! Let's check in on my vehicles..." << std::endl;
	printVehicles();
}