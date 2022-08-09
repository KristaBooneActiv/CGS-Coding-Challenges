#include "ChatMessage.h"
using namespace chat;

size_t Message::size() const
{
	return sender.length() + message.length() + sizeof(std::time_t) + 2;;
}

void Message::serialize(const Message& aMessage, char* data)
{
	size_t buffSize = aMessage.size();

	// Serialize sender name
	size_t bufferIdx = 0;
	size_t senderNameSize = aMessage.sender.length() + 1;
	memcpy(&data[bufferIdx], aMessage.sender.c_str(), senderNameSize);

	// Serialize message
	bufferIdx += senderNameSize;
	size_t messageSize = aMessage.message.length() + 1;
	memcpy(&data[bufferIdx], aMessage.message.c_str(), messageSize);

	// Serialize time
	bufferIdx += messageSize;
	memcpy(&data[bufferIdx], &aMessage.time, sizeof(std::time_t));
}

void Message::deserialize(char* data, size_t dataLength, Message& aMessage)
{
	std::string tmp;
	for (size_t i = 0; i < dataLength; ++i)
	{
		if (data[i] == '\0')
		{
			break;
		}
		else
		{
			tmp += data[i];
		}
	}
	aMessage.sender = tmp;
	size_t senderSize = tmp.length() + 1;

	std::string tmp2;
	for (size_t i = senderSize; i < dataLength; ++i)
	{
		if (data[i] == '\0')
		{
			break;
		}
		else
		{
			tmp2 += data[i];
		}
	}
	aMessage.message = tmp2;
	size_t messageSize = tmp2.length() + 1;

	size_t buffIdx = senderSize + messageSize;
	memcpy(&aMessage.time, &data[buffIdx], sizeof(std::time_t));
}