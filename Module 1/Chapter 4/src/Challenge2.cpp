#include <iostream>

#include "Challenge2.h"
#include "Car.h"

std::string Challenge2::details() const
{
	return "Define and use a Car class.";
}

void Challenge2::run()
{
	std::cout << "Creating emptyCar using an empty constructor..." << std::endl;
	Car emptyCar;
	emptyCar.Display();

	std::cout << "\nCreating myCar using a non-empty constructor..." << std::endl;
	Car myCar(2015, 70000, "Chevy");
	myCar.Display();
	
	myCar.Drive(200);
	std::cout << "After driving myCar 200 miles..." << std::endl;
	myCar.Display();
}