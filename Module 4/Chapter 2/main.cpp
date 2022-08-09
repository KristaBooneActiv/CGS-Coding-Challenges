#include <enet/enet.h>
#include <iostream>

#include "ChatHost.h"
#include "ChatManager.h"
#include "Utilities.h"

// Initialize the ENet library
// @return true if successful, otherwise false.
bool InitializeENet()
{
    // Ensure enet is initialized
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        std::cout << "An error occurred while initializing ENet." << std::endl;
        return false;
    }
    atexit(enet_deinitialize);
    return true;
}


chat::Host* CreateHostFromUserInput()
{
    std::cout << "1) Create Server " << std::endl;
    std::cout << "2) Create Client " << std::endl;

    std::optional<int> number;
    while (!number.has_value())
    {
        number = util::getNumberFromUser<int>();
        if (number != 1 && number != 2)
        {
            number.reset();
            std::cerr << "Invalid option. Please enter 1 or 2." << std::endl;
        }
    }

    if (number == 1)
    {
        return new chat::Server();
    }
    else if (number == 2)
    {
        return new chat::Client();
    }
    else
    {
        throw std::exception("This should not happen. You broke the code. RIP program.");
    }
}

int main(int argc, char** argv)
{
    // Attempt to initialize the ENet library
    if (!InitializeENet())
    {
        return EXIT_FAILURE;
    }
    
    // Create a host (server or client)
    chat::Host* hostPtr(nullptr);
    try
    {
        hostPtr = CreateHostFromUserInput();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    chat::Manager manager(hostPtr);
    manager.Run();

    return EXIT_SUCCESS;
}