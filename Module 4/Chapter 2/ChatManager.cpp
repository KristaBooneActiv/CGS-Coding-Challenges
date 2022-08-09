#include <iostream>
#include <enet/enet.h>
#include <thread>
#include <mutex>

#include "ChatHost.h"
#include "ChatManager.h"
#include "ChatMessage.h"
#include "Utilities.h"

using namespace chat;

Manager::Manager(Host* aHostPtr)
	: mHostPtr(std::move(aHostPtr))
{
	if (!mHostPtr)
	{
		throw std::exception("ERROR: Cannot create ChatManager with null host. Please initialize before passing into ChatManager.");
	}

    mHostPtr->SetSubscriber(this);
}

Manager::~Manager()
{
    delete mHostPtr;
}

bool Manager::Run()
{
	// Get the user's name
	mUsername = util::GetUserInput("Enter your name: ");

    std::cout << "Connecting to chat room..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::system("cls");

	// Spin up thread to get input for chat
    std::thread inputThread = std::thread(&chat::Manager::tListenForUserInput, this);

	// Spin up thread to process messages
    std::thread msgThread = std::thread(&chat::Manager::tListenForMessages, this);

    inputThread.join();
    msgThread.join();
    return true;
}

void Manager::OnMessageReceived(const chat::Message& aMessage)
{
    std::system("cls");

    // When a message is received, insert it into the queue
    mMessages.insert(aMessage);

    // Draw Messages
    for (const auto& m : mMessages)
    {
        struct tm timeinfo;
        localtime_s(&timeinfo, &m.time);
        std::cout << "[" << std::put_time(&timeinfo, "%H:%M.%S") << "] " << m.sender << ": " << m.message << std::endl;
    }
}

std::mutex mHostMutex;
void Manager::tListenForMessages()
{
    while (!mFinished)
    {
        /* Wait up to 1000 milliseconds for an event. */
        ENetEvent event;
        while (enet_host_service(mHostPtr->GetENetHost(), &event, 1000) > 0)
        {
            std::lock_guard<std::mutex> lock(mHostMutex);
            if (mHostPtr->ProcessesEventType(event.type))
            {
                mHostPtr->ProcessEvent(event);
            }
        }
    }
}

void Manager::tListenForUserInput()
{
    while (!mFinished)
    {
        Message message;
        message.sender = mUsername;
        message.message = util::GetUserInput("");
        message.time = std::time(nullptr);

        OnMessageReceived(message);

        size_t dataSize = message.size();
        char* data = new char[dataSize];
        Message::serialize(message, data);

        /* Create a reliable packet */
        ENetPacket* packet = enet_packet_create(data, dataSize, ENET_PACKET_FLAG_RELIABLE);

        /* Send the packet to the peer over channel id 0. */
        /* One could also broadcast the packet by         */
        std::lock_guard<std::mutex> lock(mHostMutex);
        enet_host_broadcast(mHostPtr->GetENetHost(), 0, packet);
        /* One could just use enet_host_service() instead. */
        enet_host_flush(mHostPtr->GetENetHost());
    }
}