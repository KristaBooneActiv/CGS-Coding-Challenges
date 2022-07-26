#include <iostream>
#include "Game.h"
#include "AudioManager.h"
#include "StateMachineExampleGame.h"

using namespace std;

int main()
{
	Game myGame;

	StateMachineExampleGame gameStateMachine(&myGame);

	myGame.Initialize(&gameStateMachine);

	int gameReturn = 0;
	try
	{
		myGame.RunGameLoop();
	}
	catch (const std::exception& e)
	{
		cerr << e.what() << endl;
		gameReturn = -1;
	}

	myGame.Deinitialize();

	AudioManager::DestroyInstance();

	return gameReturn;
}