#include <windows.h>
#include <conio.h>
#include "UserInputManager.h"


void UserInputManager::addInputCallback(UserInputTypeId aInputKey, Callback aCallback)
{
	mCallbacks[aInputKey] = aCallback;
}

void UserInputManager::start()
{
	mInterrupt = false;
	mThread = std::thread(&UserInputManager::getUserInput, this);
}

void UserInputManager::end()
{
	mInterrupt = true;
	if (mThread.joinable())
	{
		mThread.join();
	}
}

void UserInputManager::getUserInput()
{
	while (!mInterrupt)
	{
		int input = _getch();

		// One of the arrow keys were pressed
		if (input == inputKey::ARROW_TYPE)
		{
			input = _getch();
		}

		auto mapIter = mCallbacks.find(input);
		if (mapIter != mCallbacks.end())
		{
			mapIter->second();
		}
	}
}