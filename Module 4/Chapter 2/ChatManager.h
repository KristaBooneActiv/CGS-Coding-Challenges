#pragma once
#include <memory>
#include <set>
#include <string>

#include "ChatMessagePubSub.h"
#include "ChatMessage.h"

namespace chat
{
	class Manager : public IMessageSubscriber
	{
	public:
		Manager(Host* aHostPtr);
		~Manager();

		bool Run();
		void OnMessageReceived(const Message& aMessage) override;

	protected:
		Host*               mHostPtr{ nullptr };    /* A wrapped ENet connection   */
		std::atomic<bool>   mFinished{ false };     /* Thread-safe flag for ending */
		std::string         mUsername{ "" };        /* The chat user's name        */
		std::set<Message>   mMessages{ };           /* The queue of messages       */

		void tListenForMessages();
		void tListenForUserInput();
	};
}