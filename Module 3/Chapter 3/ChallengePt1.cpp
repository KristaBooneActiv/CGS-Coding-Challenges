#include <iostream>
#include <thread>

#include "ChallengePt1.h"

std::string ChallengePt1::details() const
{
	return "Runs a separate thread with a variable and allows user to view or quit.";
}

// ===================================================
bool shouldEndThread = false;
int threadVar = 0;

void VariableCounterThread()
{
	while (!shouldEndThread)
	{
		threadVar++;
		if (threadVar > 1000)
		{
			threadVar = 0;
		}
	}
}

void ChallengePt1::run()
{
	std::cout << "Hello Threading World!" << std::endl;


	// Pass in the function name to myThread
	// so that it is ran in a separate thread.
	//
	// Note that this thread is not guaranteed
	// to run before code after it in main().
	// 
	// UNLESS you use join()- as seen below.
	std::thread myThread(VariableCounterThread);

	while (1)
	{
		std::cout << "Press any key to display the thread counter"
			<< " or 'q' to quit" << std::endl;

		char option;
		std::cin >> option;
		if (option == 'q')
		{
			shouldEndThread = true;
			break;
		}

		std::cout << "threadVar: " << threadVar << std::endl;
	}

	myThread.join();
}