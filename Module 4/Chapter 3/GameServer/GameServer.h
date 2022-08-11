#pragma once
#include <enet/enet.h>
#include <unordered_map>

#include "NumberGuessingGame.h"
#include "GamePacket.h"

class GameServer
{
public:
	GameServer();
	~GameServer();

	void Run();

protected:
	NumberGuessingGame  mGuessingGame;

	static const int cUNSET_PLAYER_ID = INT_MAX;
	static const int cPLAYER_1_SET_ID = 1;
	static const int cPLAYER_2_SET_ID = 2;

	int mPlayer1ID{ cUNSET_PLAYER_ID };
	int mPlayer2ID{ cUNSET_PLAYER_ID };

	ENetHost*           mServerPtr;
	std::atomic<bool>   mShouldShutdown;

	void tListenForShutdown();
	void tListenForPackets();

	void BroadcastPacket(GamePacket* aPacketPtr);
	void SendPacket(GamePacket* aPacketPtr, ENetPeer* aPeerPtr);
	void ProcessPacket(PlayerGuessPacket* aPacketPtr);
};