#pragma once
#include <enet/enet.h>
#include "GamePacket.h"

class GameClient
{
public:
	GameClient();
	~GameClient();

	bool Run();

protected:
	ENetHost*          mClientPtr;
	std::atomic<bool>  mShouldShutdown;
	std::atomic<bool>  mWaitingForServerResponse;
	std::string        mGuessString;

	void tListenForInput();
	void tListenForPackets();

	bool ConnectToServer();
	void CheckAndSendGuess();
	void ProcessPacket(GamePacket* aPacketPtr);
	void PrintServerResponsePacket(ServerResponsePacket* aServerResponsePacket);
};