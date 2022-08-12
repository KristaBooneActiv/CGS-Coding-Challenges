// Here is the BeeHive challenge!!
// "Create a small program that creates 4 beehive threads that all produce honey.
// They all produce their honey after some random number of seconds between 5 - 15 seconds.
// When a beehive has produced honey it places in some static container object that holds a single instance of honey.
// If the container already has a honey instance in it then the beehives need to wait until the container is free and empty.
// Be careful that multiple threads don't try to place honey there at the same time!
// The farmer thread will take honey from the container if honey exists.
// You also need to be careful again that it is not touching the container when the beehives are touching it.
// You can represent an instance of honey as a boolean to keep things simple.
// 
// For additional challenge change the 5 - 15 second timer on the beehives to waiting to see if 1 of 4 flower objects has produced any pollen.
// The flowers can produce pollen at a set rate or random rate."
#include "Beehive.h"
#include "Flower.h"
#include "StandardOutputForThreads.h"

bool EXIT = false;
void tFarmHoney()
{
	while (!EXIT)
	{
		if (Beehive::GetHoney())
		{
			WriteOutputChunk("Farmer    | Harvested one honey\n\n");
		}
	}
}

int main()
{
	std::cout << "--------------------------------------------------------------------------------------------------\n"
		<< "Welcome to the Bee Simulator (new, with threads!)\n"
		<< "The farmer will try to grab some honey from some beehives,\n"
		<< "but they have to make sure the bees are not actively putting honey in, or they might get stung!\n\n"
		<< "Press 'ENTER' at any time to stop the simulator.\n"
		<< "--------------------------------------------------------------------------------------------------\n" << std::endl;

	// Set up the flowers
	size_t flowerTimerMin = 5;
	size_t flowerTimerMax = 10;
	Flower f1(flowerTimerMin, flowerTimerMax);
	Flower f2(flowerTimerMin, flowerTimerMax);
	Flower f3(flowerTimerMin, flowerTimerMax);
	Flower f4(flowerTimerMin, flowerTimerMax);

	// Set up the bees
	size_t hiveTimerMin = 5;
	size_t himeTimerMax = 15;
	Beehive b1(hiveTimerMin, himeTimerMax);
	Beehive b2(hiveTimerMin, himeTimerMax);
	Beehive b3(hiveTimerMin, himeTimerMax);
	Beehive b4(hiveTimerMin, himeTimerMax);

	// Set up the farmer
	auto farmerThread = std::thread(tFarmHoney);

	// Wait until user presses enter
	std::cin.ignore();

	WriteOutputChunk("\n\n~~~ Thanks for vising the farm ~~~\n\n");

	// Shutdown the honey production operation
	Flower::StopProduction();
	Beehive::StopProduction();
	EXIT = true;

	// Join threads
	// Honey production threads will be joined
	// as beehives are deconstructed.
	farmerThread.join();
}