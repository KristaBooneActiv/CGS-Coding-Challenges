#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

bool didQuit = false;
bool shouldDecrementLife = false;
std::mutex mutex;

struct Character
{
	float position = 0.;
	int lives = 1;

	// Atomic variable ensures that this variable is updated
	// once for each call to update it (no unknowns between threads).
	// 
	// Example use-case is for unique ID for thread-safe assignment.
	std::atomic<int> score = 0;

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
			// Create a lock guard, passing a shared
			// mutex. Once the scope of the guard is
			// left, the mutex is unlocked for use.
			//
			// Note- Only lock when/where you need to!
			std::lock_guard<std::mutex> guard(mutex);
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
			std::lock_guard<std::mutex> guard(mutex);
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

int main()
{
	std::thread InputHandler(ProcessInput);
	std::thread CharacterUpdate1(UpdateCharacter1);
	std::thread CharacterUpdate2(UpdateCharacter2);

	InputHandler.join();
	CharacterUpdate1.join();
	CharacterUpdate2.join();
}