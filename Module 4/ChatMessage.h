#pragma once
#include <ctime>
#include <string>

namespace chat
{
	struct Message
	{
		static void serialize(const Message& aMessage, char* data);
		static void deserialize(char* data, size_t dataLength, Message& aMessage);
		size_t size() const;

		std::string sender;
		std::string message;
		std::time_t time;
	};
}