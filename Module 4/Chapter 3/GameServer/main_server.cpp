#include <enet/enet.h>
#include <iostream>

#include "GameServer.h"

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

	GameServer server;
	server.Run();
	return EXIT_SUCCESS;
}