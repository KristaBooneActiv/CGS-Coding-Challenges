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
		2     /* allow up to 32 clients and/or outgoing connections */,
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
				std::cout << "Player guess received" << std::endl;
				auto packetPtr = dynamic_cast<PlayerGuessPacket*>(DeserializeGamePacket((char*)event.packet->data, event.packet->dataLength));
				if (!packetPtr)
				{
					throw std::exception("BAD PACKET RECEIVED BY THE SERVER.");
					return;
				}

				// Make sure we forward this to the other player.
				if ((int*)event.peer->data == &mPlayer1ID)
				{
					ServerMessageToPlayerPacket msg;
					msg.message = "[Player 1]: " + std::to_string(packetPtr->number);
					SendPacket(&msg, &mServerPtr->peers[1]);
				}
				else if ((int*)event.peer->outgoingPeerID == &mPlayer2ID)
				{
					ServerMessageToPlayerPacket msg;
					msg.message = "[Player 2]: " + std::to_string(packetPtr->number);
					SendPacket(&msg, &mServerPtr->peers[0]);
				}

				ProcessPacket(packetPtr);
				delete packetPtr;
				break;
			}
			case ENET_EVENT_TYPE_CONNECT:
			{
				if (mPlayer1ID == cUNSET_PLAYER_ID)
				{
					std::cout << "Player 1 connected" << std::endl;
					mPlayer1ID = cPLAYER_1_SET_ID;
					event.peer->data = (void*)(&mPlayer1ID);

					// Let player know they are player 1
					ServerMessageToPlayerPacket msg;
					msg.message = "[Server]: Welcome! You've successfully joined the server as Player 1. Waiting until Player 2 connects...";
					SendPacket(&msg, event.peer);
				}
				else if (mPlayer2ID == cUNSET_PLAYER_ID)
				{
					std::cout << "Player 2 connected" << std::endl;
					mPlayer2ID = cPLAYER_2_SET_ID;
					event.peer->data = (void*)(&mPlayer2ID);

					// Let player know they are player 2
					ServerMessageToPlayerPacket msg;
					msg.message = "[Server]: Welcome! You've successfully joined the server as Player 2.";
					SendPacket(&msg, event.peer);

					// Both players have connected. Start the game!
					ServerGameStartingPacket packet;

					std::stringstream msgSS;
					msgSS << "The Number Guessing Game is starting! Here's how it works: \n"
						<< "\tSend me a number and I'll tell you if you are right or wrong.\n"
						<< "\tYou have " << mGuessingGame.GetMaxGuesses() << " guesses to get it right.\n"
						<< "\tIf you haven't gotten it right in " << mGuessingGame.GetHowManyGuessesUntilHintsActivated() << " guesses, you will start to get hints!\n"
						<< "Ready to play? :)\n\n";
					packet.message = msgSS.str();
					BroadcastPacket(&packet);
				}
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				if ((int*)event.peer->data == &mPlayer1ID)
				{
					std::cout << "Player 1 left the game" << std::endl;
					mPlayer1ID = cUNSET_PLAYER_ID;

					// If player 2 is still here, promote them to player 1 and wait for a second player...
					if(mPlayer2ID != cUNSET_PLAYER_ID)
					{
						std::cout << "Promoting Player 2 to Player 1" << std::endl;

						mPlayer1ID = mPlayer2ID;
						mPlayer2ID = cUNSET_PLAYER_ID;

						// Send player 2 a message letting them know they are now player 1. Game restart required.
						mGuessingGame.Restart();

						ServerMessageToPlayerPacket msg;
						msg.message = "[Server]: Player 1 disconnected... You are now Player 1! The game will restart, and will begin once a second player connects...";
						SendPacket(&msg, &mServerPtr->peers[0]);
					}
					else
					{
						// Both players have disconnected
						std::cout << "All players have left the game. Shutting down..." << std::endl;
						mShouldShutdown = true;
					}
				}
				else if ((int*)event.peer->data == &mPlayer2ID)
				{
					std::cout << "Player 2 left the game" << std::endl;
					mPlayer2ID = cUNSET_PLAYER_ID;

					// Notify player 1 that player 2 disconnected. Game restart required.
					mGuessingGame.Restart();

					ServerMessageToPlayerPacket msg;
					msg.message = "[Server]: Player 2 disconnected... The game will restart, and will begin once a second player connects...";
					SendPacket(&msg, &mServerPtr->peers[0]);
				}				
				break;
			}
			default:
				continue;
			}
		}
	}
}

void GameServer::BroadcastPacket(GamePacket* aPacketPtr)
{
	std::stringstream ss;
	aPacketPtr->serialize(ss);

	std::string data = ss.str();
	size_t dsz = data.size();

	ENetPacket* packet = enet_packet_create(data.c_str(), data.size(), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(mServerPtr, 0, packet);
	enet_host_flush(mServerPtr);
}

void GameServer::SendPacket(GamePacket* aPacketPtr, ENetPeer* aPeerPtr)
{
	std::stringstream ss;
	aPacketPtr->serialize(ss);

	std::string data = ss.str();
	size_t dsz = data.size();

	ENetPacket* packet = enet_packet_create(data.c_str(), data.size(), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(aPeerPtr, 0, packet);
	enet_host_flush(mServerPtr);
}

void GameServer::ProcessPacket(PlayerGuessPacket* aPacketPtr)
{
	// Determine if player's guess is correct
	auto gameResult = mGuessingGame.IsNumber(aPacketPtr->number);

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

