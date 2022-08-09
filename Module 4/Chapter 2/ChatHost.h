#pragma once
#include <string>
#include <enet/enet.h>

#include "ChatMessagePubSub.h"

namespace chat
{
	struct Message;

	class Host : public IMessagePublisher
	{
	public:
		Host(std::string aHostname, int aPort);
		Host() = default;
		virtual ~Host();

		virtual void ProcessEvent(const ENetEvent& aEvent) = 0;
		virtual bool ProcessesEventType(ENetEventType aEventType) const = 0;

		ENetHost* GetENetHost() const { return mHostPtr; }

		void SetSubscriber(IMessageSubscriber* aSubscriberPtr) override;
		void Publish(const Message& aMessage) const override;

	protected:
		ENetHost*    mHostPtr              { nullptr };
		std::string  mHostname             { "localhost" };
		int          mPort                 { 1234 };

		IMessageSubscriber* mSubscriberPtr { nullptr };
	};

	class Client : public Host
	{
	public:
		Client();
		Client(std::string aHostname, int aPort);
		virtual ~Client() = default;

		virtual bool ProcessesEventType(ENetEventType aEventType) const override;
		virtual void ProcessEvent(const ENetEvent& aEvent) override;

	protected:
		void InitHost();
	};

	class Server : public Host
	{
	public:
		Server();
		Server(std::string aHostname, int aPort);
		virtual ~Server() = default;

		virtual bool ProcessesEventType(ENetEventType aEventType) const override;
		virtual void ProcessEvent(const ENetEvent& aEvent) override;

	protected:
		void InitHost();
		void ProcessConnectionEvent(const ENetEvent& aEvent);
		void ProcessDisconnectEvent(const ENetEvent& aEvent);
		void ProcessReceiveEvent(const ENetEvent& aEvent);

	};
}