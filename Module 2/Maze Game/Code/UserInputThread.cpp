#include <conio.h>

#include "UserInputThread.h"

constexpr int kArrowInput = 224;
constexpr int kLeftArrow = 75;
constexpr int kRightArrow = 77;
constexpr int kUpArrow = 72;
constexpr int kDownArrow = 80;
constexpr int kEscapeKey = 27;

UserInputThread::UserInputThread(Callback aCallback)
	: mThread()
	, mCallback(aCallback)
	, mInterrupt(false)
{ /* no-op */ }

void UserInputThread::start()
{
	mInterrupt = false;
	mThread = std::thread(&UserInputThread::getUserInput, this);
}

void UserInputThread::end()
{
	mInterrupt = true;
	if (mThread.joinable())
	{
		mThread.join();
	}
}

void UserInputThread::getUserInput()
{
	while (!mInterrupt)
	{
		UserInputType inputType = UserInputType::eNone;

		int input = _getch();
		int arrowInput = 0;

		// One of the arrow keys were pressed
		if (input == kArrowInput)
		{
			arrowInput = _getch();
		}

		if ((input == kArrowInput && arrowInput == kLeftArrow) ||
			(char)input == 'A' || (char)input == 'a')
		{
			inputType = eMoveLeft;
		}
		else if ((input == kArrowInput && arrowInput == kRightArrow) ||
			(char)input == 'D' || (char)input == 'd')
		{
			inputType = eMoveRight;
		}
		else if ((input == kArrowInput && arrowInput == kUpArrow) ||
			(char)input == 'W' || (char)input == 'w')
		{
			inputType = eMoveUp;
		}
		else if ((input == kArrowInput && arrowInput == kDownArrow) ||
			(char)input == 'S' || (char)input == 's')
		{
			inputType = eMoveDown;
		}
		else if (input == kEscapeKey)
		{
			inputType = eExit;
		}
		else if ((char)input == 'Z' || (char)input == 'z')
		{
			inputType = eDropKey;
		}

		// If the input type
		if (inputType != eNone)
		{
			mCallback(inputType);
		}
	}
}