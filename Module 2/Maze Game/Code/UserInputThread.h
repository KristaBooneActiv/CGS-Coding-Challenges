#pragma once
#include <functional>
#include <thread>

class GameplayState;

enum UserInputType
{
	eNone,
	eMoveUp,
	eMoveDown,
	eMoveLeft,
	eMoveRight,
	eDropKey,
	eExit
};

class UserInputThread
{
public:
	typedef std::function<void(UserInputType)> Callback;

	UserInputThread(Callback aCallback);
	~UserInputThread() = default;

	void start();
	void end();

private:
	std::thread   mThread;
	Callback      mCallback;
	bool          mInterrupt;

	void getUserInput();
};