#pragma once
#include <enet/enet.h>
#include <vector>

#include "NumberGuessingGame.h"
#include "GamePacket.h"

class GameServer
{
public:
	GameServer();
	~GameServer();

	void Run();

protected:
	NumberGuessingGame       mGuessingGame;

	ENetHost*                mServerPtr;
	std::atomic<bool>        mShouldShutdown;

	std::vector<std::string> mPlayers;

	void tListenForShutdown();
	void tListenForPackets();
	void ProcessPacket(GamePacket* aPacketPtr);
};