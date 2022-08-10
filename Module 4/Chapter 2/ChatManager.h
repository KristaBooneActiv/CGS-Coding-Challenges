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
		Host*             mHostPtr{ nullptr };          /* A wrapped ENet connection                               */
		std::string       mUsername{ "" };              /* The chat user's name                                    */
		std::set<Message> mMessages{ };                 /* The queue of messages                                   */
																							                         
		std::string       mUserMessage{ "" };           /* Buffered user message                                   */
		std::atomic<bool> mReadyToSend{ false };        /* Thread-safe flag for notifying message is ready to send */
		std::atomic<bool> mFinished{ false };           /* Thread-safe flag for ending                             */

		void tListenForMessages();
		void tListenForUserInput();
		void tSendMessageWhenReady();

		void SendMessagePacket();
	};
}