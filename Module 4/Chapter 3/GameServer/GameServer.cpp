#include <conio.h>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>

#include "GameServer.h"

GameServer::GameServer()
	: mGuessingGame()
	, mServerPtr(nullptr)
{
	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = 1234;
	mServerPtr = enet_host_create(&address /* the address to bind the server host to */,
		32     /* allow up to 32 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth    */,
		0      /* assume any amount of outgoing bandwidth    */);
	if (!mServerPtr)
	{
		throw std::exception("An error occurred while trying to create an ENet server host.");
	}
}

GameServer::~GameServer()
{
	enet_host_destroy(mServerPtr);
}

void GameServer::Run()
{
	std::thread shutdownListener(&GameServer::tListenForShutdown, this);
	std::thread packetListener(&GameServer::tListenForPackets, this);

	shutdownListener.join();
	packetListener.join();
}

void GameServer::tListenForPackets()
{
	while (!mShouldShutdown)
	{
		ENetEvent event;
		while (enet_host_service(mServerPtr, &event, 1000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_RECEIVE:
			{
				auto packetPtr = DeserializeGamePacket((char*)event.packet->data, event.packet->dataLength);
				ProcessPacket(packetPtr);
				delete packetPtr;
				break;
			}
			case ENET_EVENT_TYPE_CONNECT:
			{
				// Send welcome message to the client
				ServerWelcomePacket welcomeMsg;

				std::stringstream msgSS;
				msgSS << "Welcome to the Number Guessing Game! Here's how it works: \n"
					<< "\tSend me a number and I'll tell you if you are right or wrong.\n"
					<< "\tYou have " << mGuessingGame.GetMaxGuesses() << " guesses to get it right.\n"
					<< "\tIf you haven't gotten it right in " << mGuessingGame.GetHowManyGuessesUntilHintsActivated() << " guesses, you will start to get hints!\n"
					<< "Ready to play? :)\n\n";
				welcomeMsg.welcomeMessage = msgSS.str();

				std::stringstream ss;
				welcomeMsg.serialize(ss);

				std::string data = ss.str();
				size_t dsz = data.size();

				ENetPacket* packet = enet_packet_create(data.c_str(), data.size(), ENET_PACKET_FLAG_RELIABLE);
				enet_host_broadcast(mServerPtr, 0, packet);
				enet_host_flush(mServerPtr);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				continue;
			}
			default:
				continue;
			}
		}
	}
}

void GameServer::ProcessPacket(GamePacket* aPacketPtr)
{
	switch (aPacketPtr->type)
	{
	case GamePacketType::ePlayerGuess:
	{
		// Determine if player's guess is correct
		auto guessPtr = static_cast<PlayerGuessPacket*>(aPacketPtr);
		auto gameResult = mGuessingGame.IsNumber(guessPtr->number);

		// If there is a hint from the game, pass it on in the packet
		ServerResponsePacket* response(nullptr);
		if (gameResult.shouldGuessHigher.has_value())
		{
			response = new ServerResponseWithHintPacket(gameResult.guessWasRight,
				mGuessingGame.IsGameOver(),
				gameResult.shouldGuessHigher.value());
		}
		else
		{
			response = new ServerResponsePacket(gameResult.guessWasRight,
				mGuessingGame.IsGameOver());
		}

		// Send resulting packet to each player through broadcast message
		std::stringstream ss;
		response->serialize(ss);

		ENetPacket* packet = enet_packet_create(ss.str().c_str(), ss.str().size(), ENET_PACKET_FLAG_RELIABLE);
		enet_host_broadcast(mServerPtr, 0, packet);
		enet_host_flush(mServerPtr);

		delete response;
		break;
	}
	case GamePacketType::eServerGuessResponse:
	case GamePacketType::eServerGuessResponseWithHint:
	{
		std::cerr << "Unexpected server packet received at the server! "
			<< "There should only be one server..." << std::endl;
		break;
	}
	}
}

void GameServer::tListenForShutdown()
{
	while (!mShouldShutdown)
	{
		// Listen for escape key on the server
		if (_getch() == 27)
		{
			mShouldShutdown = true;
		}
	}
}

