#pragma once

namespace chat
{
	struct Message;

	class IMessageSubscriber
	{
	public:
		virtual void OnMessageReceived(const Message& aMessage) = 0;
	};

	class IMessagePublisher
	{
	public:
		virtual void SetSubscriber(IMessageSubscriber* aSubscriberPtr) = 0;
		virtual void Publish(const Message& aMessage) const = 0;
	};
}