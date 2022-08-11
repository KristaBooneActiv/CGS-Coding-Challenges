#include <enet/enet.h>
#include <iostream>

#include "GameClient.h"

bool InitializeENet()
{
	// Try to initialize the library
	if (enet_initialize() != 0)
	{
		std::cout << "An error occurred while initializing ENet!" << std::endl;
		return false;
	}
	atexit(enet_deinitialize);
	return true;
}

int main()
{
	if (!InitializeENet())
	{
		return EXIT_FAILURE;
	}

	GameClient server;
	bool successful = server.Run();
	return (successful ? EXIT_SUCCESS : EXIT_FAILURE);
}