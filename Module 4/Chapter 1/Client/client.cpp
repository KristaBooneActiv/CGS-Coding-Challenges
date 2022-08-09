// Challenge:
// Follow the setup demonstrated in the videos,
// (download & link to enet library) ensuring it compiles.
#include <enet/enet.h>
#include <iostream>

ENetHost* initClient()
{
	ENetHost* clientPtr;
	clientPtr = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection        */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of outgoing bandwidth */);
	if (clientPtr == NULL)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}
	return clientPtr;
}

int main()
{
	// Try to initialize the library
	if (enet_initialize() != 0)
	{
		std::cout << "An error occurred while initializing ENet!" << std::endl;
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	std::cout << "Starting client..." << std::endl;
	ENetHost* clientPtr = initClient();

	std::cout << "Press enter to continue..." << std::endl;
	std::cin.ignore();

	enet_host_destroy(clientPtr);
	return EXIT_SUCCESS;
}