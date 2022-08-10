#include <iostream>
#include <enet/enet.h>
#include <thread>
#include <mutex>
#include <conio.h>

#include "ChatHost.h"
#include "ChatManager.h"
#include "ChatMessage.h"
#include "Utilities.h"

#include "UserInputTypes.h"

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

	// Spin up thread to get input for message
    std::thread inputThread = std::thread(&chat::Manager::tListenForUserInput, this);

	// Spin up thread to process messages received
    std::thread msgThread = std::thread(&chat::Manager::tListenForMessages, this);

    // Spin up thread to send messages when ready
    std::thread sendThread = std::thread(&chat::Manager::tSendMessageWhenReady, this);

    inputThread.join();
    msgThread.join();
    sendThread.join();
    return true;
}

std::mutex mHostMutex;
std::mutex mMessageMutex;
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

    std::lock_guard<std::mutex> lock(mMessageMutex);
    std::cout << mUserMessage;
}

void Manager::SendMessagePacket()
{
    Message message;
    message.sender = mUsername;
    {
        std::lock_guard<std::mutex> lock(mMessageMutex);
        message.message = mUserMessage;
        mUserMessage.clear();
    }
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

    delete[] data;
}

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
        // Build message one character at a time
        int input = _getch();

        if (input == inputKey::ENTER)
        {
            // When user presses enter we are ready to send
            mReadyToSend = true;
        }
        else if (input == inputKey::ESCAPE)
        {
            // When user presses escape, we should exit
            mFinished = true;
        }
        else if (input == inputKey::BACKSPACE)
        {
            // Erase the last character of the input
            std::lock_guard<std::mutex> lock(mMessageMutex);
            mUserMessage.pop_back();

            // This will back up by 1 character and overwrite empty char
            std::cout << "\b \b";
        }
        else if(isprint(input))
        {
            // Any printable value should be added to the message
            char inputChar = static_cast<char>(input);
            std::cout << inputChar;

            std::lock_guard<std::mutex> lock(mMessageMutex);
            mUserMessage += inputChar;
        }
        // Otherwise, continue
    }
}

void Manager::tSendMessageWhenReady()
{
    while (!mFinished)
    {
        if (mReadyToSend)
        {
            mReadyToSend = false;
            SendMessagePacket();
        }
    }
}
