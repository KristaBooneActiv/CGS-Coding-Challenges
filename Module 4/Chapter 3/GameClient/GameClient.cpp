#include <conio.h>
#include <iostream>
#include <thread>

#include "GameClient.h"
#include "UserInputTypes.h"
#include "Utilities.h"

GameClient::GameClient()
	: mClientPtr(nullptr)
	, mWaitingForServerResponse(false)
	, mGuessString("")
{
	mClientPtr = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection        */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of outgoing bandwidth */);
	if (!mClientPtr)
	{
		throw std::exception("An error occurred while trying to create an ENet client host.");
	}
}

GameClient::~GameClient()
{
	enet_host_destroy(mClientPtr);
}

bool GameClient::ConnectToServer()
{
	ENetAddress address;
	enet_address_set_host(&address, "localhost");
	address.port = 1234;

	/* Initiate the connection, allocating the two channels 0 and 1. */
	ENetPeer* peer = enet_host_connect(mClientPtr, &address, 2, 0);
	if (!peer)
	{
		std::cerr << "No available peers for initiating an ENet connection." << std::endl;
		return false;
	}

	/* Wait up to 5 seconds for the connection attempt to succeed. */
	ENetEvent event;
	if (enet_host_service(mClientPtr, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		return true;
	}

	/* Either the 5 seconds are up or a disconnect event was */
	/* received. Reset the peer in the event the 5 seconds   */
	/* had run out without any significant event.            */
	enet_peer_reset(peer);
	std::cout << "Connection to the server failed." << std::endl;
	return false;
}

bool GameClient::Run()
{
	// Connect to the server
	if (!ConnectToServer())
	{
		return false;
	}

	std::thread packetListener(&GameClient::tListenForPackets, this);
	std::thread inputListener(&GameClient::tListenForInput, this);

	packetListener.join();
	inputListener.join();
	return true;
}

void GameClient::tListenForPackets()
{
	while (!mShouldShutdown)
	{
		/* Wait up to 1000 milliseconds for an event. */
		ENetEvent event;
		while (enet_host_service(mClientPtr, &event, 1000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
			{
				auto packetPtr = DeserializeGamePacket((char*)event.packet->data, event.packet->dataLength);
				ProcessPacket(packetPtr);
				break;
			}
			case ENET_EVENT_TYPE_CONNECT:
			case ENET_EVENT_TYPE_DISCONNECT:
				// TODO: Broadcase a message that 'so and so connected' or disconnected
				continue;
			default:
				continue;
			}
		}
	}
}

void GameClient::ProcessPacket(GamePacket* aPacketPtr)
{
	switch (aPacketPtr->type)
	{
	case GamePacketType::ePlayerGuess:
	{
		// Write out that someone guessed a number
		auto guessPtr = static_cast<PlayerGuessPacket*>(aPacketPtr);
		std::cout << "The number " << guessPtr->number << " was guessed." << std::endl;
		break;
	}
	case GamePacketType::eServerGuessResponse:
	{
		// Determine if player's guess is correct
		auto guessPtr = static_cast<ServerResponsePacket*>(aPacketPtr);
		PrintServerResponsePacket(guessPtr);
		mWaitingForServerResponse = false;
		break;
	}
	case GamePacketType::eServerGuessResponseWithHint:
	{
		auto guessPtr = static_cast<ServerResponseWithHintPacket*>(aPacketPtr);
		PrintServerResponsePacket(guessPtr);

		std::cout << "Hint: You should guess a " << (guessPtr->guessHigher ? "higher" : "lower") << " number." << std::endl;
		mWaitingForServerResponse = false;
		break;
	}
	case GamePacketType::eServerWelcomeMessage:
	{
		auto welcomePtr = static_cast<ServerWelcomePacket*>(aPacketPtr);
		std::cout << welcomePtr->welcomeMessage << std::endl;
		break;
	}
	}
}

void GameClient::PrintServerResponsePacket(ServerResponsePacket* aServerResponsePacket)
{
	std::cout << "That was " << (aServerResponsePacket->guessWasCorrect ? "correct! " : "incorrect... ");
	if (aServerResponsePacket->gameOver && aServerResponsePacket->guessWasCorrect)
	{
		std::cout << "You win!" << std::endl;
	}
	else if (aServerResponsePacket->gameOver)
	{
		std::cout << "You lose. :(" << std::endl;
	}
	else
	{
		std::cout << "Guess again." << std::endl;
	}
}

void GameClient::tListenForInput()
{
	while (!mShouldShutdown)
	{
		if (_kbhit()) {

			// Stores the pressed key in ch
			char ch = _getch();
			if (ch == inputKey::ESCAPE)
			{
				mShouldShutdown = true;
			}
			else if (ch == inputKey::BACKSPACE)
			{
				if(!mGuessString.empty())
				{
					mGuessString.pop_back();
				}
				std::cout << "\b \b";
			}
			else if (ch == inputKey::ENTER)
			{
				std::cout << std::endl;
				CheckAndSendGuess();
			}
			else
			{
				mGuessString += ch;
				std::cout << ch;
			}
		}
	}
}

void GameClient::CheckAndSendGuess()
{
	if (mWaitingForServerResponse)
	{
		std::cerr << "You must wait until the game has responded before guessing again!" << std::endl;
		mGuessString.clear();
		return;
	}

	if (!util::isNumber(mGuessString))
	{
		std::cerr << "That's not a number! Try again." << std::endl;
		mGuessString.clear();
		return;
	}

	PlayerGuessPacket guessPacket(std::stoi(mGuessString));

	std::stringstream ss;
	guessPacket.serialize(ss);

	std::string data = ss.str();
	size_t dsz = data.size();

	ENetPacket* packet = enet_packet_create(data.c_str(), data.size(), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(mClientPtr, 0, packet);
	enet_host_flush(mClientPtr);

	mGuessString.clear();
	mWaitingForServerResponse = true;
}
