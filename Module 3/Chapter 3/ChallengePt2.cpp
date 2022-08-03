#include <chrono>
#include <thread>
#include <iostream>

#include "ChallengePt2.h"

std::string ChallengePt2::details() const
{
	return "Further explores multi-threading.";
}

bool didQuit = false;
bool shouldDecrementLife = false;

struct Character
{
	float position = 0.;
	int score = 0;
	int lives = 1;

	void DisplayStats()
	{
		std::cout << "Lives: " << lives << std::endl;
	}
};

Character player;

void UpdateCharacter1()
{
	while (!didQuit)
	{
		if (shouldDecrementLife)
		{
			if (player.lives > 0)
			{
				// We can sleep here to mimic doing some other
				// work in this thread before editing the shared variable
				//std::this_thread::sleep_for(std::chrono::milliseconds(500));

				// The yield method tells the OS to go work on another
				// thread instead of this one (another way to 'sleep')
				//std::this_thread::yield();
				player.lives--;
			}
		}
	}
}

void UpdateCharacter2()
{
	while (!didQuit)
	{
		if (shouldDecrementLife)
		{
			if (player.lives > 0)
			{
				// We can sleep here to mimic doing some other
				// work in this thread before editing the shared variable
				//std::this_thread::sleep_for(std::chrono::milliseconds(500));

				// The yield method tells the OS to go work on another
				// thread instead of this one (another way to 'sleep')
				//std::this_thread::yield();
				player.lives--;
			}
		}
	}
}

void ProcessInput()
{
	while (!didQuit)
	{
		std::cout << "Enter a value:" << std::endl;
		std::cout << "'a' to decrement player life" << std::endl;
		std::cout << "'d' to display player stats" << std::endl;
		std::cout << "'q' to quit" << std::endl;

		char input;
		std::cin >> input;

		if (input == 'a')
		{
			shouldDecrementLife = true;
		}
		else if (input == 'd')
		{
			player.DisplayStats();
		}
		else if (input == 'q')
		{
			didQuit = true;
		}
		else
		{
			std::cerr << "Bad value. Try again." << std::endl;
		}
	}
}

void ChallengePt2::run()
{
	std::thread InputHandler(ProcessInput);
	std::thread CharacterUpdate1(UpdateCharacter1);
	std::thread CharacterUpdate2(UpdateCharacter2);

	InputHandler.join();
	CharacterUpdate1.join();
	CharacterUpdate2.join();
}