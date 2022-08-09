// Challenge:
// Follow the setup demonstrated in the videos,
// (download & link to enet library) ensuring it compiles.
#include <enet/enet.h>
#include <iostream>

ENetHost* initServer()
{
	ENetAddress address;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	address.port = 1234;
	ENetHost* serverPtr = enet_host_create(&address /* the address to bind the server host to */,
		32     /* allow up to 32 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth    */,
		0      /* assume any amount of outgoing bandwidth    */);
	if (serverPtr == NULL)
	{
		fprintf(stderr,
			"An error occurred while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}
	return serverPtr;
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

	std::cout << "Starting server..." << std::endl;
	ENetHost* serverPtr = initServer();

	std::cout << "Press enter to continue..." << std::endl;
	std::cin.ignore();

	enet_host_destroy(serverPtr);
	return EXIT_SUCCESS;
}