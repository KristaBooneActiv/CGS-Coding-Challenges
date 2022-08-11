#pragma once
#include <string>
#include <sstream>

// ===================================================
// Enum for packet types
enum GamePacketType 
{
	ePlayerGuess,
	eServerWelcomeMessage,
	eServerGuessResponse,
	eServerGuessResponseWithHint
};

// ===================================================
// Base GamePacket
struct GamePacket
{
	GamePacketType type;

	GamePacket(GamePacketType aType)
		: type(aType) {}

	virtual void serialize(std::ostream& out)
	{
		out.write((char*)(&type), sizeof(GamePacketType));
	}
	virtual void deserialize(std::istream& in)
	{
		in.read((char*)(&type), sizeof(GamePacketType));
	}
};

// ===================================================
// Player Packet Types
struct PlayerGuessPacket : public GamePacket
{
	int number {INT_MAX};

	PlayerGuessPacket() : GamePacket(ePlayerGuess) {}
	PlayerGuessPacket(int aNumber)
		: GamePacket(ePlayerGuess)
		, number(aNumber) {}

	virtual void serialize(std::ostream& out) override
	{
		GamePacket::serialize(out);
		out.write((char*)(&number), sizeof(int));
	}
	virtual void deserialize(std::istream& in) override
	{
		GamePacket::deserialize(in);
		in.read((char*)(&number), sizeof(int));
	}
};

// ===================================================
// Server Packet Types
struct ServerWelcomePacket : public GamePacket
{
	std::string welcomeMessage {""};

	ServerWelcomePacket() : GamePacket(eServerWelcomeMessage) {}
	ServerWelcomePacket(const std::string& aWelcomeMessage)
		: GamePacket(eServerWelcomeMessage)
	    , welcomeMessage(aWelcomeMessage) {}

	virtual void serialize(std::ostream& out) override
	{
		GamePacket::serialize(out);
		out.write(welcomeMessage.c_str(), welcomeMessage.size() + 1);
	}
	virtual void deserialize(std::istream& in) override
	{
		GamePacket::deserialize(in);
		std::getline(in, welcomeMessage, '\0');
	}
};

struct ServerResponsePacket : public GamePacket
{
	bool guessWasCorrect {false};
	bool gameOver {true};

	ServerResponsePacket() : GamePacket(eServerGuessResponse) {}
	ServerResponsePacket(bool aGuessWasCorrect, bool aGameOver)
		: GamePacket(eServerGuessResponse)
	    , guessWasCorrect(aGuessWasCorrect)
	    , gameOver(aGameOver) {}

	virtual void serialize(std::ostream& out) override
	{
		GamePacket::serialize(out);
		out.write((char*)(&guessWasCorrect), sizeof(bool));
		out.write((char*)(&gameOver), sizeof(bool));
	}
	virtual void deserialize(std::istream& in) override
	{
		GamePacket::deserialize(in);
		in.read((char*)(&guessWasCorrect), sizeof(bool));
		in.read((char*)(&gameOver), sizeof(bool));
	}
};

struct ServerResponseWithHintPacket : public ServerResponsePacket
{
	bool guessHigher {false};

	ServerResponseWithHintPacket() : ServerResponsePacket() { type = eServerGuessResponseWithHint; };
	ServerResponseWithHintPacket(bool aGuessWasCorrect, bool aGameOver, bool aGuessHigher)
		: ServerResponsePacket(aGuessWasCorrect, aGameOver)
		, guessHigher(aGuessHigher)
	{
		type = eServerGuessResponseWithHint;
	}

	virtual void serialize(std::ostream& out) override
	{
		ServerResponsePacket::serialize(out);
		out.write((char*)(&guessHigher), sizeof(bool));
	}
	virtual void deserialize(std::istream& in) override
	{
		ServerResponsePacket::deserialize(in);
		in.read((char*)(&guessHigher), sizeof(bool));
	}
};

// ===================================================
// Main Method For Serialization
template<class T>
T* DeserializeGamePacketOfType(std::istringstream& in)
{
	T* packet = new T();
	packet->deserialize(in);
	return packet;
}

static GamePacket* DeserializeGamePacket(const char* aData, size_t aDataLength)
{
	GamePacketType type;
	memcpy(&type, aData, sizeof(type));

	std::istringstream ss(std::string(aData, aDataLength));
	switch (type)
	{
	case ePlayerGuess:
	{
		return DeserializeGamePacketOfType<PlayerGuessPacket>(ss);
	}
	case eServerWelcomeMessage:
	{
		return DeserializeGamePacketOfType<ServerWelcomePacket>(ss);
	}
	case eServerGuessResponse:
	{
		return DeserializeGamePacketOfType<ServerResponsePacket>(ss);
	}
	case eServerGuessResponseWithHint:
	{
		return DeserializeGamePacketOfType<ServerResponseWithHintPacket>(ss);
	}
	default:
	{
		return nullptr;
	}
	}
}