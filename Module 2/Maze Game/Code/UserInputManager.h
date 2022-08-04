#pragma once
#include <functional>
#include <thread>
#include <unordered_map>

#include "UserInputTypes.h"

class UserInputManager
{
public:
	typedef std::function<void(void)> Callback;
	typedef std::unordered_map<UserInputTypeId, Callback> Map;

	UserInputManager() = default;
	~UserInputManager() = default;

	void addInputCallback(UserInputTypeId aInputKey,
		                  Callback aCallback);
	void start();
	void end();

private:
	// A structure that maps user input to a function call
	Map           mCallbacks{};

	// The thread that is running to read the user's input
	std::thread   mThread{};

	// An interrupt that can be used in the thread
	bool          mInterrupt{ false };

	// Loop to get user input (ran by mThread once started)
	void getUserInput();
};